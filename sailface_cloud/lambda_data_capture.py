"""Receive a message from Rock7's HTTP call and persist to an s3 bucket."""
import json
from datetime import datetime
import boto3

from sailface_cloud.status_parser import SailFaceStatusMessageParser

print('Loading function')
s3 = boto3.resource("s3")

bucket_name = "sailface-data"
s3_raw_path = "raw/"
s3_processed_path = "processed/"

def respond(err, res=None):
    return {
        'statusCode': '400' if err else '200',
        'body': err.message if err else json.dumps(res),
        'headers': {
            'Content-Type': 'application/json',
        },
    }

def save_raw_message_to_s3(event):
    """Receive a POST payload and persist to a S3 bucket."""
    filename = f"message-{datetime.now().isoformat()}.json"
    s3_path = f"raw/{filename}"

    encoded_string = event['body'].encode("utf-8")

    s3.Bucket(bucket_name).put_object(Key=s3_path, Body=encoded_string)

def parse_message(self, message_body):
    parser = SailFaceStatusMessageParser()
    return parser.parse(message_body)

def load_existing_messages_from_s3(message_date):
    bucket = s3.Bucket(bucket_name)

    expected_file_path = f'{s3_processed_path}/{message_date.isoformat()}.json'

    try:
        file_contents = bucket.Object(expected_file_path).get()['Body'].read()
    except botocore.exceptions.ClientError as ex:
        if ex.response['Error']['Code'] == 'NoSuchKey':
            return None
        else:
            raise ex

    return json.load(file_contents)

def save_processed_messages_to_s3(messages, message_date):
    filename = f"{message_date}.json"
    s3_path = f"processed/{filename}"

    encoded_string = event['body'].encode("utf-8")

    s3.Bucket(bucket_name).put_object(Key=s3_path, Body=encoded_string)


def lambda_handler(event, context):
    """Receive a POST payload and persist to a S3 bucket."""

    operation = event['httpMethod']
    if operation != 'POST':
        return respond(ValueError('Unsupported method "{}"'.format(operation)))

    # STORE THE RAW MESSAGE
    save_raw_message_to_s3(event)

    # Parse the message a store into a processed JSON file
    parsed_message = parse_message(event['body'].encode("utf-8"))

    transmit_date = datetime.fromisoformat(parsed_message['transmit_time']).date()

    existing_messages = load_existing_messages_from_s3(transmit_date)

    if parsed_message['message_key'] not in existing_messages:
        existing_messages[parsed_message['message_key']] = parsed_message

    save_processed_messages_to_s3(existing_messages, transmit_date)

    return respond(None, {
        "result": "success"
    })
