from flask import Flask, request, jsonify, render_template, redirect
import message_printer
from config import PORT, HOST, PASSWORD, ID, DEBUG

ms = message_printer.MessagePrinter()
app = Flask(__name__)

clientid = ID
password_home = PASSWORD

@app.route('/', methods=["GET"]) 
def mainpage():
    return render_template('index.html'), 200


@app.route('/validate_data', methods=['POST', 'GET'])
def validate_data():
    if request.method == "POST":
        data = request.json 
        # request: {"myid": int:5812, "password": int:12123131..., "ip": "127.0.0.1"}

        ms.info_message(f"{data}")
        if 'myid' in data and 'password' in data and 'ip' in data:
            myid = data['myid']
            password = data['password']
            ip = data['ip']
            if myid == clientid:
                if password == password_home:
                    response = ({"message": "Data is valid", "premium": "1"}, 200)
                    print("========!!! WINNER WINNER !!!===========")
                else:
                    response = ({"error": "Invalid password"}, 400)
            else: 
                response = ({"error": "Invalid id"}, 400)
        else:
            response = ({"error": "Data is invalid"}, 400)
        print(data)
        return jsonify(response[0]), response[1]
    else: 
        return redirect("/", 200)
if __name__ == '__main__':
    app.run(host=HOST, port=PORT, debug=DEBUG)