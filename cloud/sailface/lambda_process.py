from sailface.status_parser import SailFaceStatusMessageParser

def lambda_handler(event, context):
    """Receive a POST payload and persist to a S3 bucket."""

    message = '' # HOW DO IT GET THE MESSAGE FROM THE S3 FILE?

    s = SailFaceStatusMessageParser()
    values = s.parse(message)

    for key, value in values.items():
        print('%s:\t%20s' % (key, value))
    print()

    json.dumps(values)
