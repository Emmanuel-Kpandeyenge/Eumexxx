import uuid
from typing import Optional #
from fastapi import Depends, Request # Importing necessary classes and functions from FastAPI for handling dependencies and requests
from fastapi_users import BaseUserManager, FastAPIUsers, UUIDIDMixin, models # Importing necessary classes and modules from FastAPI Users for user management
from fastapi_users.authentication import (
    AuthenticationBackend,
    BearerTransport,
    JWTStrategy
)

from fastapi_users.db import SQLAlchemyUserDatabase # Importing the SQLAlchemyUserDatabase class for integrating FastAPI Users with SQLAlchemy
from app.db import User, get_user_db

SECRET = "12131wsdfgsdfgsdfgsdfgsdfg" # Defining a secret key for JWT authentication

class UserManager(UUIDIDMixin, BaseUserManager[User, uuid.UUID]): # Defining a UserManager class that inherits from UUIDIDMixin and BaseUserManager for managing user operations
    reset_password_token_secret = SECRET # Setting the secret key for password reset tokens
    verification_token_secret = SECRET # Setting the secret key for email verification tokens

    async def on_after_register(self, user: User, request: Optional[Request] = None): # Defining an asynchronous method that is called after a user registers
        print(f"User {user.id} has registered.") # Printing a message to the console indicating that a user has registered

    async def on_after_forgot_password(self, user: User, token: str, request: Optional[Request] = None): # Defining an asynchronous method that is called after a user requests a password reset
        print(f"User {user.id} has forgot their password. Reset token: {token}") # Printing a message to the console indicating that a user has requested a password reset along with the reset token

    async def on_after_request_verify(self, user: User, token: str, request: Optional[Request] = None): # Defining an asynchronous method that is called after a user requests email verification
        print(f"Verification requested for user {user.id}. Verification token: {token}") # Printing a message to the console indicating that a user has requested email verification along with the verification token  

async def get_user_manager(user_db: SQLAlchemyUserDatabase = Depends(get_user_db)): # Defining an asynchronous function to get the user manager for FastAPI Users integration
    yield UserManager(user_db) # Yielding an instance of the UserManager class for use in FastAPI Users integration

bearer_transport = BearerTransport(tokenUrl="auth/jwt/login") # Creating an instance of BearerTransport for handling JWT authentication with the specified token URL

def get_jwt_strategy():
    return JWTStrategy(secret=SECRET, lifetime_seconds=3600) # Defining a function to get the JWT strategy for authentication with the specified secret and token lifetime

auth_backend = AuthenticationBackend(
    name="jwt", # Setting the name of the authentication backend to "jwt"
    transport=bearer_transport, # Setting the transport method for the authentication backend to the previously defined bearer_transport
    get_strategy=get_jwt_strategy # Setting the function to get the authentication strategy to the previously defined get_jwt_strategy function
)

fastapi_users = FastAPIUsers[User, uuid.UUID](get_user_manager, [auth_backend]) # Creating an instance of FastAPIUsers for managing user operations with the specified user manager and authentication backend
current_active_user = fastapi_users.current_user(active=True) # Defining a dependency to get the current active user for use in route functions