"""

    Swarmy Library
    by: c0mplh4cks
    version 1.0.2

    This code is focused on functional programming to keep it simple.
    Use of objects is minimized as much as possible.


    + Description +

        This is the library for the Swarmy robot containing
        functions to control the robot remotely. Two of the
        functions are examples to show what can be done with
        the software.


"""





# === Importing Library's === #
from socket import socket, AF_INET, SOCK_DGRAM, timeout      # Import needed for networking.
from time import sleep





# === Networking === #
def send(msg, addr):                    # Networking Functions which
    s = socket(AF_INET, SOCK_DGRAM)     # allows to communicate with
                                        # the Swarmy. Takes message
                                        # as string and the address
    try:                                # of the Swarmy as tuple (ip, port).
        s.sendto(msg.encode(), addr)
    except KeyboardInterrupt:
        pass

    s.close()
    return 1



def send_recv(msg, addr, port):         # Same Function as above
    s = socket(AF_INET, SOCK_DGRAM)     # except this one requires
    s.settimeout(0.05)                  # the Swarmy to respond.
    s.bind(("0.0.0.0", port))

    while True:
        try:
            s.sendto(msg.encode(), addr)
            rsp, addr = s.recvfrom(1024)
            s.close()
            return int( rsp.decode() )
        except timeout:
            sleep(0.05)
        except KeyboardInterrupt:
            break

    s.close()
    return 0





# === Standard I/O Functions === #
def motor_left(speed, addr):            # Takes speed as int and
    msg = "A{};".format(speed)          # addr as tuple (ip, port)
    return send(msg, addr)


def motor_right(speed, addr):           # Same parameters as
    msg = "B{};".format(speed)          # motor_left function.
    return send(msg, addr)


def light_switch(bool, addr, port):     # Takes bool as boolean
    msg = "L{};".format(bool)           # and addr as tuple (ip, port).
    return send_recv(msg, addr, port)


def light_sensor(id, addr, port):       # Takes id of sensor as
    msg = "I{};".format(id)             # int and addr as
    return send_recv(msg, addr, port)   # tuple (ip, port).


def led_display(id, r, g, b, addr):
    msg = "C{};{};{};{};".format(id, r, g, b)
    return send(msg, addr)


def text_display(first, second, third, addr):
    msg = "D{};{};{};".format(first, second, third)
    return send(msg, addr)





# === Behavior Example Functions === #
def follow(speed, treshold_min, treshold_max, addr, port):      # Takes speed as int, treshold
                                                                # as int and addr as tuple (ip, port).

    for id in range(8):                                 # Repeat the code for each sensor.
        value = light_sensor(id, addr, port)            # Save sensor value as a variable.

        if treshold_min < value < treshold_max:         # Check if sensor value is between
                                                        # the tresholds.

            if id == 0:     # North         # If the ID of the sensor is 0,
                l = speed                   # then use the given speeds.
                r = speed                   # Goes the same way for the other
                                            # ID conditions.
            elif id == 1:   # North East
                l = speed
                r = 0

            elif id == 2:   # East
                l = speed
                r = -speed

            elif id == 3:   # South East
                l = speed
                r = -speed

            elif id == 4:   # South
                l = -speed
                r = speed

            elif id == 5:   # South West
                l = -speed
                r = speed

            elif id == 6:   # West
                l = -speed
                r = speed

            elif id == 7:   # North West
                l = 0
                r = speed

            break               # This break prevents the code
                                # from unnecessery repeats.

        else:
            l, r = 0, 0         # If the IR values aren't between
                                # the tresholds, stop moving.

    motor_left(l, addr)     # Send the left and right
    motor_right(r, addr)    # motor speeds to the Swarmy.





def keep_distance(speed, treshold, addr, port): # Takes speed as int, treshold
                                                # as int and addr as tuple (ip, port).

    for id in range(8):                         # Repeat the code for each sensor.
        value = light_sensor(id, addr, port)    # Save sensor value as a variable.

        if value < treshold:                # Check if sensor value is getting lower
                                            # then the treshold value.

            if id == 0:     # North         # If the ID of the sensor is 0,
                l = -speed                  # then use the given speeds.
                r = -speed                  # Goes the same way for the other
                                            # ID conditions.
            elif id == 1:   # North East
                l = -speed
                r = speed

            elif id == 2:   # East
                l = -speed
                r = speed

            elif id == 3:   # South East
                l = 0
                r = speed

            elif id == 4:   # South
                l = speed
                r = speed

            elif id == 5:   # South West
                l = speed
                r = 0

            elif id == 6:   # West
                l = speed
                r = -speed

            elif id == 7:   # North West
                l = speed
                r = -speed

            break           # This break prevents the code
                            # from unnecessery repeats.

        else:
            l, r = 0, 0     # If the Swarmy isn't to close
                            # to an other, stop moving.

    motor_left(l, addr)     # Send the left and right
    motor_right(r, addr)    # motor speeds to the Swarmy.
