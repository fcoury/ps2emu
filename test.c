#include <stdio.h>

int main()
{
  unsigned char data = 0x00;
  unsigned char i;
  unsigned char bit = 0x01;
  unsigned char parity = 1;

  unsigned char value = 0x1c;

  for (i = 0; i < 8; i++)
  {
    data = data | bit;
    bit = bit << 1;
    parity = parity ^ (data & 0x01);
    printf("%d: data: %d bit: %d parity: %d\n", i, data, bit, parity);
  }

  printf("data: %d bit: %d parity: %d\n", data, bit, parity);

  return 0;
}
