from fastapi import FastAPI, HTTPException, File, UploadFile, Form, Depends # importing FastAPI class from fastapi module and HTTPException for handling exceptions in the API
from app.schemas import PostCreate, PostResponse, UserRead, UserUpdate, UserCreate # importing the PostCreate schema from the schemas module in the app package
from app.db import Post, create_db_and_tables, get_async_session, User # importing the Post model, create_db_and_tables function, and get_async_session function from the db module in the app package
from sqlalchemy.ext.asyncio import AsyncSession # importing AsyncSession for handling asynchronous database sessions
from contextlib import asynccontextmanager
from sqlalchemy import select # importing the select function from SQLAlchemy for constructing SQL queries
from app.s3 import upload_file_to_s3, delete_file_from_s3 # importing the upload_file_to_s3 and delete_file_from_s3 functions from the s3 module in the app package
import uuid # importing the uuid module for generating unique identifiers
from app.users import auth_backend, current_active_user, fastapi_users # importing the auth_backend and current_active_user from the users module in the app package for handling user authentication and retrieving the current active user

@asynccontextmanager
async def lifespan(app: FastAPI):
    await create_db_and_tables() # creating the database and tables before the application starts
    yield # yielding control back to the application to run
    # any cleanup code can be added here if needed after the application shuts down

app = FastAPI(lifespan=lifespan) # creating an instance of FastAPI class and assigning it to the variable app

app.include_router(fastapi_users.get_auth_router(auth_backend), prefix="/auth/jwt", tags=["auth"]) # including the authentication router from FastAPI Users for handling JWT authentication with the specified prefix and tags
app.include_router(fastapi_users.get_register_router(UserRead, UserCreate), prefix="/auth", tags=["auth"])
app.include_router(fastapi_users.get_reset_password_router(), prefix="/auth", tags=["auth"]) # including the password reset router from FastAPI Users for handling password reset operations with the specified prefix and tags
app.include_router(fastapi_users.get_verify_router(UserRead), prefix="/auth", tags=["auth"]) # including the email verification router from FastAPI Users for handling email verification operations with the specified prefix and tags
app.include_router(fastapi_users.get_users_router(UserRead, UserUpdate), prefix="/users", tags=["users"]) # including the users router from FastAPI Users for handling user-related operations with the specified prefix and tags



@app.post("/upload")
async def upload_file(
    file: UploadFile = File(...), # defining a file parameter that accepts an uploaded file using FastAPI's File class
    caption: str = Form(""), 
    user: User = Depends(current_active_user), # defining a user parameter that depends on the current_active_user function to retrieve the currently authenticated user
    session: AsyncSession = Depends(get_async_session) # defining a session parameter that depends on the get_async_session function to provide an asynchronous database session
):
    file_url = await upload_file_to_s3(file)
    post = Post(
    caption=caption,
    url=file_url,
    file_type="photo",
    file_name=file.filename,
    user_id=user.id
)
    session.add(post) # adding the new post to the database session
    await session.commit() # committing the transaction to save the post in the database
    await session.refresh(post) # refreshing the post instance to get the updated data from the database
    return post


@app.get("/feed")
async def get_feed(
    session: AsyncSession = Depends(get_async_session), # defining a session parameter that depends on the get_async_session function to provide an asynchronous database session
    user: User = Depends(current_active_user) # defining a user parameter that depends on the current_active_user function to retrieve the currently authenticated user
):
    result = await session.execute(select(Post).order_by(Post.created_at.desc())) # executing a query to select all posts ordered by creation date in descending order
    posts = [row[0] for row in result.all()] # fetching all results and extracting the Post instances from the query result

    result = await session.execute(select(User)) # executing a query to select the current user from the database using their ID
    users = [row[0] for row in result.all()] # fetching all results and extracting the User instances from the query result 
    user_dict = {u.id: u.email for u in users} # creating a dictionary to map user IDs to their email addresses for easy lookup
    posts_data = []
    for post in posts:
        posts_data.append(
            {
                "id": str(post.id),
                "user_id": str(post.user_id),
                "caption": post.caption,
                "url": post.url,
                "file_type": post.file_type,
                "file_name": post.file_name,
                "created_at": post.created_at.isoformat(),
                "is_owner": post.user_id == user.id,
                "email": user_dict.get(post.user_id, "Unknown")
            }
        )
    return {"posts": posts_data} # returning the list of posts as a JSON response

@app.delete("/posts/{post_id}")
async def delete_post(post_id: str, session: AsyncSession = Depends(get_async_session), user: User = Depends(current_active_user)  ):    # defining a delete_post function that takes a post_id as a path parameter and a session parameter that depends on the get_async_session function to provide an asynchronous database session
    try: 
        post_uuid = uuid.UUID(post_id) 
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid ID")
    # Fetch the post from the database
    result = await session.execute( 
        select(Post).where(Post.id == post_uuid)
    )
    # Extract the post from the query result
    post = result.scalars().first()
    # Check if the post exists
    if not post:
        raise HTTPException(status_code=404, detail="Post not found")

    if post.user_id != user.id:
        raise HTTPException(status_code=403, detail="Not authorized to delete this post")
    
    # STEP 1: delete from S3
    delete_file_from_s3(post.url)

    # STEP 2: delete from DB
    await session.delete(post)
    await session.commit()

    return {"success": True, "message": "Deleted from S3 and DB"}




