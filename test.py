data = 0
bit = 1
parity = 1
value = int('1c', 16)

for i in range(8):
  data = data | bit
  bit = (bit << 1) % 255
  parity = parity ^ (data & 1)
  print('{0}: data: {1} bit: {2} parity: {3}'.format(i, data, bit, parity))

print('data: {0} bit: {1} parity: {2}'.format(data, bit, parity))
