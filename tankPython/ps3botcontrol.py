#!/usr/bin/python2.7 python
'''
ps3 controller for serial com using xbee to control tank robot.
'''

import serial
import pygame, sys, time
from pygame.locals import *

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()
screen = pygame.display.set_mode((400,300))
pygame.display.set_caption('Hello World')
ser = serial.Serial('/dev/ttyUSB0',9600) #set to your xbee serial port
interval = 0.01
dir = "x"

# get count of joysticks=1, axes=27, buttons=19 for DualShock 3

joystick_count = pygame.joystick.get_count()
numaxes = joystick.get_numaxes()
numbuttons = joystick.get_numbuttons()


loopQuit = False
while loopQuit == False:

    ser.write('%s\n' % dir)
    
    # test joystick axes
    # 1:-1 = left forward 1:0.2 = left reverse
    # 3:-1 = right forward, 3:0.2 = right reverse
    # 1:0 and 3:0 = stop for both
    
    if joystick.get_axis(1) < 0 and joystick.get_axis(3) < 0:
        dir = "w"
        print("forward")
    elif joystick.get_axis(1) > 0 and joystick.get_axis(3) > 0:
        dir = "s"
        print("reverse")
    elif joystick.get_axis(1) < 0 and joystick.get_axis(3) > 0:
        dir = "d"
        print("right turn")
    elif joystick.get_axis(1) > 0 and joystick.get_axis(3) < 0:
        dir = "a"
        print("left turn")
    elif joystick.get_axis(1) == 0 and joystick.get_axis(3) < 0:
        dir = "q"
        print("slow right turn")
    elif joystick.get_axis(1) < 0 and joystick.get_axis(3) == 0:
        dir = "e"
        print("slow left turn")
        
    #elif joystick.get_axis(1) > 0 and joystick.get_axis(3) == 0:
    #    dir = "c"
    #    print("rev slow right turn")
    #elif joystick.get_axis(1) == 0 and joystick.get_axis(3) < 0:
    #    dir = "z"
    #    print("rev slow left turn")
        
    elif joystick.get_axis(1) == 0 and joystick.get_axis(3) == 0:
        dir = "x"
        print("stopped")
        

        


    #test controller buttons
    #outstr = ""
    #for i in range (0,numbuttons):
    #    button = joystick.get_button(i)
    #    outstr = outstr + str(i) + ";" + str(button) + "|"
    #print(outstr)
        
    for event in pygame.event.get():
        if event.type == QUIT:
            loopQuit = True
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                loopQuit = True
                
    pygame.display.update()
    time.sleep(interval)

pygame.quit()
sys.exit()
