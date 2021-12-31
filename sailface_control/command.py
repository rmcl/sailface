import requests

ROCKBLOCK_MESSAGE_ENDPOINT = 'https://rockblock.rock7.com/rockblock/MT'

class CommandMessage(NamedTuple):
    latitude: float
    longitude: float
    prop_power: int


class SailFaceAdmin:
    """Send commands to SailFace via Iridium SBD."""

    def load_credentials(self):
        pass

    def send_message(self, hex_data):
        """Send a message to an Iridium device via Rock7

        https://docs.rockblock.rock7.com/reference/testinput
        """
        resp = requests.post(
            ROCKBLOCK_MESSAGE_ENDPOINT,
            params={
                'imei': '',
                'username': '',
                'password' : '',
                'data': hex_data
            },
            headers={
                'Accept': 'text/plain'
            })

        print(resp.status_code)
        print(resp.text)


    def build_command_via_user_prompt(self) -> CommandMessage:
        new_command = CommandMessage()
        new_command.latitude = float(
            input("Enter waypoint latitude (0 to retain current waypoint)"))
        new_command.longitude = float(
            input("Enter waypoint longitude (0 to retain current waypoint)"))
        new_command.prop_power = int(
            input("Enter prop speed (0-255, -1 to retain current value)"))

        return new_command

    def send_command(self):
        command = self.build_command_via_user_prompt()
