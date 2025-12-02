import serial
import time
import threading
from flask import Flask, render_template
from flask_socketio import SocketIO

ser = serial.Serial('COM7', 9600, timeout=1)
time.sleep(2)

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

@app.route("/")
def index():
    return render_template("index.html")

def ler_serial():
    """Fica lendo a serial e enviando ao navegador em tempo real."""
    while True:
        try:
            if ser.in_waiting > 0:
                linha = ser.readline().decode(errors="ignore").strip()
                socketio.emit("arduino_data", {"texto": linha})
        except:
            pass

thread = threading.Thread(target=ler_serial)
thread.daemon = True
thread.start()

if __name__ == "__main__":
    socketio.run(app, host="localhost", port=5000)
