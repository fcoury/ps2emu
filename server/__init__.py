from keyboard import Keyboard
from debug import Debug
import serial

def main():
  ser = serial.Serial("/dev/cu.usbmodem14241", 9600)
  ser.flushInput

  Debug.start(ser)
  Keyboard.start(ser)
