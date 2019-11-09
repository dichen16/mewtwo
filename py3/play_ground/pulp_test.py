import json, time, urllib3
import certifi
import ssl
import sys

# disable warnings
urllib3.disable_warnings()

# url for checking pulp status
url = "https://10.0.151.108/pulp/api/v2/status/"

# cert_none
http = urllib3.PoolManager(cert_reqs='CERT_NONE')

# send the request
res = http.request('GET', url)

# get response string
str_res = res.data.decode("utf-8") 

print(str_res)


