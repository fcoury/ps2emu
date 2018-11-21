from threading import Thread
from pynput.keyboard import Key, KeyCode, Listener

import serial
ser = serial.Serial("/dev/cu.usbmodem141111", 9600)
ser.flushInput

def read_serial():
  while True:
    print("<<", end=' ')
    print(ser.readline())

def chomp(x):
    if x.endswith("\r\n"): return x[:-2]
    if x.endswith("\n") or x.endswith("\r"): return x[:-1]
    return x

def split(s, n):
    if len(s) < n:
        return []
    else:
        return [s[:n]] + split(s[n:], n)

thread = Thread(target = read_serial)
thread.start()

convertions = {}
with open('scancodes.txt', 'r') as f:
  for l in f:
    [key, make, brk] = chomp(l).split('|')
    try:
      dictKey = key if len(key) < 2 else Key[key]
      convertions[dictKey] = { 'make': make, 'brk': brk }
    except KeyError:
      print("Skipped {0}".format(key))

def getItem(key):
  if isinstance(key, KeyCode):
    hexCode = convertions.get(key.char.lower())
  else:
    hexCode = convertions.get(key)

  return hexCode

def write(hexCode):
  for h in split(hexCode, 2):
    code = int(h, 16)
    ser.write([code])

def on_press(key):
  hexCode = getItem(key)

  if hexCode is not None:
    print('{0} pressed - send 0x{1}'.format(key, hexCode['make']))
    write(hexCode['make'])
  else:
    print('{0} pressed - no correspondent key')

def on_release(key):
  hexCode = getItem(key)

  if hexCode is not None:
    print('{0} released - send 0x{1}'.format(key, hexCode['brk']))
    write(hexCode['brk'])
  else:
    print('{0} pressed - no correspondent key')

with Listener(on_press=on_press, on_release=on_release) as listener:
  listener.join()
