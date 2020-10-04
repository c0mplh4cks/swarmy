# === Importing Library's === #
from readchar import readchar
from time import sleep
from sys import path
import swarmy_lib as s



# === Main === #
speed = 1000
turnspeed = 300

devices = [
    ("", 8888),
]




try:

    while True:

        key = readchar()

        if key == "\x03": raise KeyboardInterrupt
        elif key == "w": a, b = speed, speed
        elif key == "s": a, b = -speed, -speed
        elif key == "d": a, b = turnspeed, -turnspeed
        elif key == "a": a, b = -turnspeed, turnspeed
        elif key == " ": a, b = 0, 0

        for device in devices:
            s.motor_left(a, device)
            s.motor_right(b, device)

    sleep(0.01)



except KeyboardInterrupt:
    for device in devices:
        s.motor_left(0, device)
        s.motor_right(0, device)
