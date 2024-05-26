import requests

data = {'key': 'example', 'value': 123, 'myid':2}

url = 'https://e08f-188-162-0-253.ngrok-free.app/validate_data'
response = requests.post(url, json=data)
print(response.json())
