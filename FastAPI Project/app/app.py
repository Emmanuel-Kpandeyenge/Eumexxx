from fastapi import FastAPI, HTTPException, File, UploadFile, Form, Depends
from fastapi.middleware.cors import CORSMiddleware
from app.schemas import PostCreate, PostResponse, UserRead, UserUpdate, UserCreate, CommentCreate
from app.db import Post, Like, Comment, create_db_and_tables, get_async_session, User
from sqlalchemy.ext.asyncio import AsyncSession
from contextlib import asynccontextmanager
from sqlalchemy import select, func
from app.s3 import upload_file_to_s3, delete_file_from_s3
import uuid
from app.users import auth_backend, current_active_user, fastapi_users

IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg"}
VIDEO_EXTENSIONS = {".mp4", ".avi", ".mov", ".mkv", ".webm"}


def get_media_type(file_name: str, content_type: str | None = None) -> str:
    if content_type:
        if content_type.startswith("image/"):
            return "image"
        if content_type.startswith("video/"):
            return "video"

    extension = f".{file_name.rsplit('.', 1)[-1].lower()}" if "." in file_name else ""
    if extension in IMAGE_EXTENSIONS:
        return "image"
    if extension in VIDEO_EXTENSIONS:
        return "video"

    raise HTTPException(status_code=400, detail="Unsupported file type")


def get_display_name(user: User) -> str:
    if user.first_name and user.last_name:
        return f"{user.first_name} {user.last_name}"
    if user.first_name:
        return user.first_name
    return user.email


@asynccontextmanager
async def lifespan(app: FastAPI):
    await create_db_and_tables()
    yield


app = FastAPI(lifespan=lifespan)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(fastapi_users.get_auth_router(auth_backend), prefix="/auth/jwt", tags=["auth"])
app.include_router(fastapi_users.get_register_router(UserRead, UserCreate), prefix="/auth", tags=["auth"])
app.include_router(fastapi_users.get_reset_password_router(), prefix="/auth", tags=["auth"])
app.include_router(fastapi_users.get_verify_router(UserRead), prefix="/auth", tags=["auth"])
app.include_router(fastapi_users.get_users_router(UserRead, UserUpdate), prefix="/users", tags=["users"])


@app.post("/users/me/avatar")
async def upload_avatar(
    file: UploadFile = File(...),
    current_user: User = Depends(current_active_user),
    session: AsyncSession = Depends(get_async_session)
):
    if not file.content_type or not file.content_type.startswith("image/"):
        raise HTTPException(status_code=400, detail="Only images are allowed")

    if current_user.avatar_url:
        delete_file_from_s3(current_user.avatar_url)

    avatar_url = await upload_file_to_s3(file)

    result = await session.execute(select(User).where(User.id == current_user.id))
    user = result.scalars().first()
    user.avatar_url = avatar_url
    await session.commit()

    return {"avatar_url": avatar_url}


@app.post("/upload")
async def upload_file(
    file: UploadFile = File(...),
    caption: str = Form(""),
    user: User = Depends(current_active_user),
    session: AsyncSession = Depends(get_async_session)
):
    file_type = get_media_type(file.filename, file.content_type)
    file_url = await upload_file_to_s3(file)
    post = Post(
        caption=caption,
        url=file_url,
        file_type=file_type,
        file_name=file.filename,
        user_id=user.id
    )
    session.add(post)
    await session.commit()
    await session.refresh(post)
    return post


@app.get("/feed")
async def get_feed(
    session: AsyncSession = Depends(get_async_session),
    user: User = Depends(current_active_user)
):
    result = await session.execute(select(Post).order_by(Post.created_at.desc()))
    posts = result.scalars().all()

    if not posts:
        return {"posts": []}

    post_ids = [p.id for p in posts]

    like_counts_result = await session.execute(
        select(Like.post_id, func.count(Like.id).label("cnt"))
        .where(Like.post_id.in_(post_ids))
        .group_by(Like.post_id)
    )
    like_counts = {row.post_id: row.cnt for row in like_counts_result}

    comment_counts_result = await session.execute(
        select(Comment.post_id, func.count(Comment.id).label("cnt"))
        .where(Comment.post_id.in_(post_ids))
        .group_by(Comment.post_id)
    )
    comment_counts = {row.post_id: row.cnt for row in comment_counts_result}

    user_likes_result = await session.execute(
        select(Like.post_id)
        .where(Like.user_id == user.id)
        .where(Like.post_id.in_(post_ids))
    )
    user_liked_ids = {row[0] for row in user_likes_result}

    users_result = await session.execute(select(User))
    user_dict = {u.id: u for u in users_result.scalars().all()}

    posts_data = []
    for post in posts:
        file_type = post.file_type
        if file_type not in {"image", "video"}:
            file_type = get_media_type(post.file_name)

        post_user = user_dict.get(post.user_id)
        posts_data.append({
            "id": str(post.id),
            "user_id": str(post.user_id),
            "caption": post.caption,
            "url": post.url,
            "file_type": file_type,
            "file_name": post.file_name,
            "created_at": post.created_at.isoformat(),
            "is_owner": post.user_id == user.id,
            "email": post_user.email if post_user else "Unknown",
            "display_name": get_display_name(post_user) if post_user else "Unknown",
            "avatar_url": post_user.avatar_url if post_user else None,
            "like_count": like_counts.get(post.id, 0),
            "comment_count": comment_counts.get(post.id, 0),
            "liked_by_me": post.id in user_liked_ids,
        })
    return {"posts": posts_data}


@app.delete("/posts/{post_id}")
async def delete_post(
    post_id: str,
    session: AsyncSession = Depends(get_async_session),
    user: User = Depends(current_active_user)
):
    try:
        post_uuid = uuid.UUID(post_id)
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid ID")

    result = await session.execute(select(Post).where(Post.id == post_uuid))
    post = result.scalars().first()

    if not post:
        raise HTTPException(status_code=404, detail="Post not found")
    if post.user_id != user.id:
        raise HTTPException(status_code=403, detail="Not authorized to delete this post")

    delete_file_from_s3(post.url)
    await session.delete(post)
    await session.commit()

    return {"success": True, "message": "Deleted from S3 and DB"}


@app.post("/posts/{post_id}/like")
async def like_post(
    post_id: str,
    session: AsyncSession = Depends(get_async_session),
    user: User = Depends(current_active_user)
):
    try:
        post_uuid = uuid.UUID(post_id)
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid ID")

    result = await session.execute(select(Post).where(Post.id == post_uuid))
    if not result.scalars().first():
        raise HTTPException(status_code=404, detail="Post not found")

    existing = await session.execute(
        select(Like).where(Like.post_id == post_uuid, Like.user_id == user.id)
    )
    if existing.scalars().first():
        raise HTTPException(status_code=409, detail="Already liked")

    session.add(Like(post_id=post_uuid, user_id=user.id))
    await session.commit()

    count_result = await session.execute(
        select(func.count(Like.id)).where(Like.post_id == post_uuid)
    )
    return {"liked": True, "like_count": count_result.scalar()}


@app.delete("/posts/{post_id}/like")
async def unlike_post(
    post_id: str,
    session: AsyncSession = Depends(get_async_session),
    user: User = Depends(current_active_user)
):
    try:
        post_uuid = uuid.UUID(post_id)
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid ID")

    result = await session.execute(
        select(Like).where(Like.post_id == post_uuid, Like.user_id == user.id)
    )
    like = result.scalars().first()
    if not like:
        raise HTTPException(status_code=404, detail="Not liked")

    await session.delete(like)
    await session.commit()

    count_result = await session.execute(
        select(func.count(Like.id)).where(Like.post_id == post_uuid)
    )
    return {"liked": False, "like_count": count_result.scalar()}


@app.get("/posts/{post_id}/comments")
async def get_comments(
    post_id: str,
    session: AsyncSession = Depends(get_async_session),
    user: User = Depends(current_active_user)
):
    try:
        post_uuid = uuid.UUID(post_id)
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid ID")

    result = await session.execute(
        select(Comment).where(Comment.post_id == post_uuid).order_by(Comment.created_at.asc())
    )
    comments = result.scalars().all()

    if not comments:
        return {"comments": []}

    user_ids = {c.user_id for c in comments}
    users_result = await session.execute(select(User).where(User.id.in_(user_ids)))
    users = {u.id: u for u in users_result.scalars().all()}

    return {
        "comments": [
            {
                "id": str(c.id),
                "post_id": str(c.post_id),
                "user_id": str(c.user_id),
                "content": c.content,
                "created_at": c.created_at.isoformat(),
                "display_name": get_display_name(users[c.user_id]) if c.user_id in users else "Unknown",
                "avatar_url": users[c.user_id].avatar_url if c.user_id in users else None,
                "is_owner": c.user_id == user.id,
            }
            for c in comments
        ]
    }


@app.post("/posts/{post_id}/comments")
async def add_comment(
    post_id: str,
    body: CommentCreate,
    session: AsyncSession = Depends(get_async_session),
    user: User = Depends(current_active_user)
):
    try:
        post_uuid = uuid.UUID(post_id)
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid ID")

    result = await session.execute(select(Post).where(Post.id == post_uuid))
    if not result.scalars().first():
        raise HTTPException(status_code=404, detail="Post not found")

    comment = Comment(post_id=post_uuid, user_id=user.id, content=body.content)
    session.add(comment)
    await session.commit()
    await session.refresh(comment)

    return {
        "id": str(comment.id),
        "post_id": str(comment.post_id),
        "user_id": str(comment.user_id),
        "content": comment.content,
        "created_at": comment.created_at.isoformat(),
        "display_name": get_display_name(user),
        "is_owner": True,
    }


@app.delete("/comments/{comment_id}")
async def delete_comment(
    comment_id: str,
    session: AsyncSession = Depends(get_async_session),
    user: User = Depends(current_active_user)
):
    try:
        comment_uuid = uuid.UUID(comment_id)
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid ID")

    result = await session.execute(select(Comment).where(Comment.id == comment_uuid))
    comment = result.scalars().first()

    if not comment:
        raise HTTPException(status_code=404, detail="Comment not found")
    if comment.user_id != user.id:
        raise HTTPException(status_code=403, detail="Not authorized")

    await session.delete(comment)
    await session.commit()

    return {"success": True}
