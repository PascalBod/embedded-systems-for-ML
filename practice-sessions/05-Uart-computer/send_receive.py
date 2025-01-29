"""
    RTOS for ML presentation - solution to exercise 05.
    Copyright (C) 2024  Pascal Bodin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    
    
    
    This application connects to the SERIAL_PORT serial port, and then sends
    any character entered by the user over the serial port.
    
    Due to the use of termios and tty modules, the application runs on Linux
    only.
    
"""

import serial
import signal
import sys
import termios
import threading
import tty

SERIAL_PORT = "/dev/ttyACM0"
VERSION = "send_receive v0.1.0"

def signal_handler(sig, frame):
    global prev_attr
    print("\nRestoring terminal attributes...")
    # Restore terminal attributes
    termios.tcsetattr(fd, termios.TCSAFLUSH, prev_attr)

def send_char(ser):
    """Reads a character from the keyboard and sends it"""
    while True:
        c = sys.stdin.read(1)
        ser.write(bytes(c, encoding='utf-8'))
        
def receive_char(ser):
    """Waits for a character from the board and prints it"""
    while True:
        c = ser.read(1)
        # Do not add a line feed.
        print(c.decode('utf-8'), end='', flush=True)
        
print(VERSION)        
        
# Connect to the board.
ser = serial.Serial(SERIAL_PORT)
ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.xonxoff = False
ser.rtscts = False
ser.dsrdtr = False

# Intercept CTRL-C before modifying terminal attributes.
signal.signal(signal.SIGINT, signal_handler)

# Save original terminal attributes.
fd = sys.stdin.fileno()
prev_attr = termios.tcgetattr(fd)
# Disable terminal input buffering.
tty.setcbreak(fd)

send_thread = threading.Thread(target=send_char, args=(ser,), daemon=True)
send_thread.start()

receive_thread = threading.Thread(target=receive_char, args=(ser,), daemon=True)
receive_thread.start()

# Wait until CTRL-C is entered.
signal.pause()


