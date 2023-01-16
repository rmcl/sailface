from sailface.config import WIFI_CREDS
from sailface.network import do_connect_wifi

print("Hello, world!")


do_connect_wifi(WIFI_CREDS[0])

