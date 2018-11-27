from pynput.keyboard import Key, KeyCode, Listener
from utils import chomp, split
import os

UNSHIFT_FROM = '!@#$%ˆ&*()_+|~{}:"<>?'
UNSHIFT_TO   = '1234567890-=\\`[];\',./'

class Keyboard:
  @classmethod
  def start(cls, serial):
    print('Starting...')
    instance = cls(serial)
    with Listener(on_press=instance.on_press, on_release=instance.on_release) as listener:
      listener.join()

  def __init__(self, serial):
    self.ser = serial
    self.enabled = False
    self.keyMappings = {}
    self.keyScanMap = {}

    missing = []

    dir_path = os.path.dirname(os.path.realpath(__file__))
    with open(dir_path + '/scancodes.txt', 'r') as f:
      for l in f:
        [key, make, brk] = chomp(l).split('|')
        try:
          dictKey = key if len(key) < 2 else Key[key]
          self.keyScanMap[dictKey] = { 'make': make, 'break': brk }
        except KeyError:
          missing.append(key)

    with open(dir_path + '/mappings.txt', 'r') as f:
      for l in f:
        [kFrom, kTo] = chomp(l).split('|')
        dictKey = kFrom if len(kFrom) < 2 else Key[kFrom]
        self.keyMappings[dictKey] = kTo

    if len(missing) > 0:
      print('  missing keys: {0}'.format(', '.join(missing)))

  def getUnshifted(self, key):
    pos = UNSHIFT_FROM.find(key)
    if pos > -1:
      print('key {0} pos {1} ret {2}'.format(key, pos, UNSHIFT_TO[pos]))
      return UNSHIFT_TO[pos]
    return key

  def getItem(self, inKey):
    key = inKey

    mappedTo = self.keyMappings.get(key)
    if mappedTo is not None:
      key = Key[mappedTo]
    hexCode = self.keyScanMap.get(key)

    return hexCode

  def write(self, hexCode):
    for h in split(hexCode, 2):
      code = int(h, 16)
      self.ser.write([code])

  def xlate(self, key):
    if isinstance(key, KeyCode):
      return self.getUnshifted(key.char.lower())

    return key

  def on_press(self, inKey):
    key = self.xlate(inKey)
    hexCode = self.getItem(key)

    print('{0} pressed ({1})'.format(key, hexCode))

    if not self.enabled:
      return

    if hexCode is not None:
      print('{0} pressed - send 0x{1}'.format(key, hexCode['make']))
      self.write(hexCode['make'])
    else:
      print('{0} pressed - no correspondent key'.format(key))

  def on_release(self, inKey):
    key = self.xlate(inKey)

    print('{0} released'.format(key))

    if key == Key.page_up:
      self.enabled = not self.enabled
      print('** Keytracking enabled' if self.enabled else '** Keytrack disabled')

    hexCode = self.getItem(key)

    if not self.enabled:
      return

    if hexCode is not None:
      print('{0} released - send 0x{1}'.format(key, hexCode['break']))
      self.write(hexCode['break'])
    else:
      print('{0} release - no correspondent key'.format(key))
