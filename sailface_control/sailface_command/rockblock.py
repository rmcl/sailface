import json
import requests

ROCKBLOCK_MESSAGE_ENDPOINT = 'https://rockblock.rock7.com/rockblock/MT'

class RockblockApiError(Exception):
    pass


class RockblockAPI:
    """Send commands to SailFace via Iridium SBD."""

    def __init__(self, rockblock_creds_path=None):
        if not rockblock_creds_path:
            self.creds_path = 'rockblock_creds.json'
        else:
            self.creds_path = rockblock_creds_path

        self.creds = self.load_credentials()

    def load_credentials(self) -> dict:
        with open(self.creds_path, 'r') as fp:
            return json.load(fp)

    def send_message(self, hex_data):
        """Send a message to an Iridium device via Rock7

        https://docs.rockblock.rock7.com/reference/testinput
        """
        resp = requests.post(
            ROCKBLOCK_MESSAGE_ENDPOINT,
            params={
                'imei': self.creds['imei'],
                'username': self.creds['username'],
                'password' : self.creds['password'],
                'data': hex_data
            },
            headers={
                'Accept': 'text/plain'
            })

        if resp.status_code != 200:
            raise Exception('Unexpected status code', resp.text)

        result = resp.text.split(',')
        status = result[0]
        if status == 'OK':
            mt_id = int(result[1])
            return mt_id

        elif status == 'FAILED':
            try:
                error_code = int(result[1])
                error_message = result[2]
            except:
                raise RockblockApiError('Could not parse API failed response')

            raise RockblockApiError(error_code, error_message)
        else:
            raise RockblockApiError('API returned unknown response format')