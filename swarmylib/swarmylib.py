"""

 Swarmy Library  -  c0mplh4cks


"""





# === Importing Dependencies === #
from socket import socket, AF_INET, SOCK_DGRAM, timeout
from struct import pack
from time import sleep







# === Networking === #
class Swarmy:
    def __init__(self, addr):
        self.addr = addr
        self.sock = socket(AF_INET, SOCK_DGRAM)
        self.sock.settimeout(0.5)
        self.id = 0


    def send(self, msg):
        errors = 0
        while errors < 3:

            self.id = (self.id + 1) %256

            packet = b"".join([
                pack(">B", self.id),
                msg.encode(),
            ])
            self.sock.sendto(packet, self.addr)

            try:
                resp, addr = self.sock.recvfrom(2)

            except timeout:
                errors += 1
                continue

            data, id = resp

            if id == self.id:
                return data
            else:
                errors += 1


        return 0



    def motor_left(self, speed=0):
        msg = f"A{ speed };"
        return self.send(msg)


    def motor_right(self, speed=0):
        msg = f"B{ speed };"
        return self.send(msg)


    def light_switch(self, bool=0):
        msg = f"L{ bool };"
        return self.send(msg)


    def light_sensor(self, id=0):
        msg = f"I{ id };"
        return self.send(msg)


    def led_display(self, id, r=0, g=0, b=0):
        msg = f"C{ id };{ r };{ g };{ b };"
        return self.send(msg)


    def led_brightness(self, brightness):
        msg = f"W{ brightness};"
        return self.send(msg)


    def text_display(self, first="", second="", third=""):
        msg = f"D{ first };{ second };{ third };"
        return self.send(msg)
