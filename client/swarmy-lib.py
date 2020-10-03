"""

    Swarmy Library
    by: c0mplh4cks

    This code is focused on functional programming to keep it simple.
    Use of objects is minimized as much as possible.


    + Description +

        This is the library for the Swarmy robot containing
        functions to control the robot remotely. Two of the
        functions are examples to show what can be done with
        the software.



    * Changes which need to be made for this to work. *

        - NodeMcu must use UDP instead of TCP
        - Swarmy must be able to turn on/off the IR lights
                1 for lights on and 0 for lights off
        - Messages send back from Swarmy to laptop must
                only contain one integer.
        - Swarmy must respond the IR sensor data
                corrosponding to the requested
                sensor ID.


"""





# === Importing Library's === #
from socket import socket, AF_INET, SOCK_DGRAM      # Import needed for networking.
from time import sleep                              # Import needed for timing.





# === Networking === #
def send(msg, addr):                    # Networking Functions which
    s = socket(AF_INET, SOCK_DGRAM)     # allows to communicate with
    s.bind(("0.0.0.0", 4242))           # the Swarmy. Takes message
                                        # as string and the address
    s.sendto(msg.encode(), addr)        # of the Swarmy as tuple (ip, port).
    rsp, addr = s.recvfrom(1024)

    s.close()
    return int( rsp.decode() )





# === Standard I/O Functions === #
def motor_left(speed, addr):            # Takes speed as int and
    msg = "A{};".format(speed)          # addr as tuple (ip, port)
    return send(msg, addr)


def motor_right(speed, addr):           # Same parameters as
    msg = "B{};".format(speed)          # motor_left function.
    return send(msg, addr)


def light_switch(bool, addr):           # Takes bool as boolean
    msg = "L{};".format(bool)           # and addr as tuple (ip, port).
    return send(msg, addr)


def light_sensor(id, addr):             # Takes id of sensor as
    msg = "I{};".format(id)             # int and addr as
    return send(msg, addr)              # tuple (ip, port).





# === Behavior Example Functions === #
def follow(speed, treshold_min, treshold_max, addr)


    for id in range(8):
        value = light_sensor(id, addr)

        if treshold_min < value < treshold_max:

            if id == 0:     # North         # If the ID of the sensor of the low value
                l = speed                   # is 0, than use the given speeds.
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





def keep_distance(speed, treshold, addr):   # Takes speed as int, treshold
                                            # as int and addr as tuple (ip, port).

    for id in range(8):                     # Repeat the code for each sensor.
        value = light_sensor(id, addr)      # Save sensor value as a variable.

        if value < treshold:                # Check if sensor value is getting lower
                                            # than treshold.

            if id == 0:     # North         # If the ID of the sensor of the low value
                l = -speed                  # is 0, than use the given speeds.
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