import keyboard
import mouse
import serial
from time import sleep

# Replace 'COM3' with the actual port name of your serial device
ser = serial.Serial('COM3', 115200)

# Key Dictionary
key_mapping = ["b", "j", "w", "a", "s", "d"]

while True:
    data = ser.read(4)  # Read a single byte from the serial device
    command = format(int(data), '011b')
    if int(command[0]):
        mouse.press('left')
    else:
        mouse.release('left')
    mouse.move(10*(int(command[4]) - int(command[2])), 10*(int(command[3]) - int(command[1])), absolute=False, duration=0.01)
    for i in range(6):
        if int(command[i+5]):
            keyboard.press(str(key_mapping[i]))
        else:
            keyboard.release(str(key_mapping[i]))