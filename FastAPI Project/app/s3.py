import boto3
import os
import uuid
from dotenv import load_dotenv

load_dotenv()

# ONE client only
s3 = boto3.client(
    "s3",
    aws_access_key_id=os.getenv("AWS_ACCESS_KEY_ID"),
    aws_secret_access_key=os.getenv("AWS_SECRET_ACCESS_KEY"),
    region_name="us-east-1"
)

# ONE bucket only
BUCKET_NAME = "fastapi-uploads-123"


# UPLOAD
async def upload_file_to_s3(file):
    file_bytes = await file.read()

    file_key = f"{uuid.uuid4()}_{file.filename}"

    s3.put_object(
        Bucket=BUCKET_NAME,
        Key=file_key,
        Body=file_bytes,
        ContentType=file.content_type
    )

    return f"https://{BUCKET_NAME}.s3.amazonaws.com/{file_key}"


# DELETE
def delete_file_from_s3(file_url: str):
    key = file_url.split("/")[-1]

    s3.delete_object(
        Bucket=BUCKET_NAME,
        Key=key
    )