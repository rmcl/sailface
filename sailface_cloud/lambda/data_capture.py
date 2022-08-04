"""Receive a message from Rock7's HTTP call and persist to an s3 bucket."""
import json
from datetime import datetime

import boto3

print('Loading function')
s3 = boto3.resource("s3")


def respond(err, res=None):
    return {
        'statusCode': '400' if err else '200',
        'body': err.message if err else json.dumps(res),
        'headers': {
            'Content-Type': 'application/json',
        },
    }

def lambda_handler(event, context):
    """Receive a POST payload and persist to a S3 bucket."""

    operation = event['httpMethod']
    if operation != 'POST':
        return respond(ValueError('Unsupported method "{}"'.format(operation)))

    bucket_name = "sailface-data"
    filename = "message-%s.json" % (datetime.now().isoformat())
    s3_path = "raw/%s" % filename

    encoded_string = event['body'].encode("utf-8")

    s3.Bucket(bucket_name).put_object(Key=s3_path, Body=encoded_string)

    return respond(None, {
        "result": "success"
    })
