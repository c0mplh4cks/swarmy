"""

 Swarmy Library  -  c0mplh4cks


"""





# === Importing Dependencies === #
from socket import socket, AF_INET, SOCK_STREAM, timeout
from threading import Thread
from struct import pack
from time import sleep, time







# === Networking === #
class Swarmy(Thread):
    def __init__(self, addr):
        super().__init__()

        self.sock = socket(AF_INET, SOCK_STREAM)
        self.sock.connect(addr)
        self.sock.settimeout(0.2)

        self.daemon = True

        self.tasks = []
        self.id = 0

        self.ir = [0, 0, 0, 0, 0, 0, 0, 0]

        self.start()



    def add_task(self, action, parameters):
        self.tasks.append( [action, parameters] )



    def run(self):
        t = time()

        while True:
            
            if len(self.tasks) > 0:
                action, parameters = self.tasks[0]
                self.tasks = self.tasks[1:]

                self.sock.send( action(*parameters).encode() + b"\x00" )

                try:
                    packet = self.sock.recv(1)
                    if len(packet) == 8:
                        for i in len(self.ir):
                            self.ir[i] = packet[i]

                except (timeout):
                    pass


            elif (time() - t) > 0.5:
                t = time()
                for i in range(8):
                    self.add_task(self.sensor(i))



    def motor(self, lspeed=0, rspeed=0):
        self.add_task(self.motorl, [lspeed])
        self.add_task(self.motorr, [rspeed])


    def motorl(self, speed=0):
        return f"A{ speed };\x00"


    def motorr(self, speed=0):
        return f"B{ speed };\x00"


    def sensor(self, id=0):
        return f"I{ id };\x00"


    def switch(self, bool=0):
        return f"L{ bool };\x00"


    def rgb(self, id=0, r=0, g=0, b=0):
        return f"C{ id };{ r };{ g };{ b };\x00"


    def bright(self, brightness):
        return f"W{ brightness};\x00"


    def text(self, first="", second="", third=""):
        return f"D{ first };{ second };{ third };\x00"
