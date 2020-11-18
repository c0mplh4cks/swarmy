"""

    Swarmy Library
    by: c0mplh4cks
    version 1.3.0


    + Description +

        This is the library for the Swarmy robot containing
        functions to control the robot remotely.


"""





# === Importing Library's === #
from socket import socket, AF_INET, SOCK_DGRAM, timeout     # Import required for networking.
from time import sleep                                      # Import required for time based delays.





# === Networking === #
# Requires a tuple containing the IP and PORT as (IP, PORT).
class Swarmy:                                               # Swarmy Object containing communication functions.
    def __init__(self, addr):
        self.addr = addr                                    # A tuple containing the IP and PORT (IP, PORT).
        self.sock = socket(AF_INET, SOCK_DGRAM)             # Socket object required for communication.
        self.sock.settimeout(0.05)                          # Timeout for evading code obstructions.


    # Requires the msg needed to be send as a string.
    def send(self, msg):                                    # Send method which gets used when
        try:                                                # sending a message (msg) when it's
            self.sock.sendto(msg.encode(), self.addr)     # not required to receive data.

        except KeyboardInterrupt:
            pass

        return 1


    # Requires the msg needed to be send as a string.
    def send_recv(self, msg):                                   # Send and receive method which
        while True:                                             # gets used when sending a message
            try:                                                # (msg) when it's required to receive
                self.sock.sendto(msg.encode(), self.addr)     # data from the Swarmy.
                resp, addr = self.sock.recvfrom(1024)
                return int( resp.decode() )

            except timeout:                         # Timout exceptions preventing
                sleep(0.05)                         # obstructions when failing to
                                                    # receive a response.
            except KeyboardInterrupt:
                break

        return 0



    # Requires a speed in the range from -255 to 255 as int.
    def motor_left(self, speed=0):          # Method to control the
        msg = f"A{ speed };"                 # speed of the left motor.
        return self.send(msg)


    # Requires a speed in the range from -255 to 255 as int.
    def motor_right(self, speed=0):         # Method to control the
        msg = f"B{ speed };"                 # speed of the right motor.
        return self.send(msg)


    # Requires a 0 as int for off or a 1 as int for on.
    def light_switch(self, bool=0):         # Method to switch the
        msg = f"L{ bool };"                  # IR LEDs on/off.
        return self.send_recv(msg)


    # Requires a int ranging from 0 to 7 for the id of the IR sensor.
    def light_sensor(self, id=0):           # Method to receive the
        msg = f"I{ id };"                   # data of the IR sensors.
        return self.send_recv(msg)


    # Requires a int ranging from 0 to 3 for the id of the RGB LED
    # followed by the red, green and blue values as three integers.
    def led_display(self, id, r=0, g=0, b=0):       # Method to set a color
        msg = f"C{ id };{ r };{ g };{ b };"         # per RGB LED.
        return self.send(msg)


    # Requires three strings for the first second and third row of
    # the text display.
    def text_display(self, first="", second="", third=""):      # Method to set the
        msg = f"D{ first };{ second };{ third }"                # text of the display
        return self.send(msg)                                   # per row.
