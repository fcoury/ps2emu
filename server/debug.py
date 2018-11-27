from termcolor import colored
from threading import Thread
from utils import chomp, split

class Debug:
  @classmethod
  def start(cls, serial):
    instance = cls(serial)
    thread = Thread(target = instance.read_serial)
    thread.start()

  def __init__(self, serial):
    self.serial = serial

  def read_serial(self):
    while True:
      print(colored(chomp(self.serial.readline().decode("utf-8")), 'yellow'))

