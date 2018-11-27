#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "ps2dev.h" // to emulate a PS/2 device

// Orange = 2
// Blue = 3
// Red = 5V (3 in)
// Black = GND (4 in)
// EXT Power, USB for COM only

const int DataPin = 2;
const int ClkPin = 3;
const int LedPin = 13;

PS2dev keyboard(ClkPin, DataPin); // PS2dev object (2:data, 3:clock)
int enabled = 0; // pseudo variable for state of "keyboard"
boolean serialConnected = false;
int incomingByte = 0;

class KbdRptParser : public KeyboardReportParser
{
  void PrintKey(uint8_t mod, uint8_t key);

protected:
  void OnControlKeysChanged(uint8_t before, uint8_t after);

  void OnKeyDown  (uint8_t mod, uint8_t key);
  void OnKeyUp  (uint8_t mod, uint8_t key);
  void OnKeyPressed(uint8_t key);
};

void kbSend(uint8_t key)
{
  Serial.print("Sending ");
  Serial.println(key, HEX);
  keyboard.write(key);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{
  Serial.println("Control keys changed");
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  Serial.print("ASCII: ");
  Serial.println((char)key);
};

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  switch(key) {
  case 0x29:
    kbSend(0xf0);
    kbSend(0x76);
    break;
  case 0x3a:
    kbSend(0xf0);
    kbSend(0x05);
    break;
  case 0x3b:
    kbSend(0xf0);
    kbSend(0x06);
    break;
  case 0x3c:
    kbSend(0xf0);
    kbSend(0x04);
    break;
  case 0x3d:
    kbSend(0xf0);
    kbSend(0x0c);
    break;
  case 0x3e:
    kbSend(0xf0);
    kbSend(0x03);
    break;
  case 0x3f:
    kbSend(0xf0);
    kbSend(0x0b);
    break;
  case 0x40:
    kbSend(0xf0);
    kbSend(0x83);
    break;
  case 0x41:
    kbSend(0xf0);
    kbSend(0x0a);
    break;
  case 0x42:
    kbSend(0xf0);
    kbSend(0x01);
    break;
  case 0x43:
    kbSend(0xf0);
    kbSend(0x09);
    break;
  case 0x44:
    kbSend(0xf0);
    kbSend(0x78);
    break;
  case 0x45:
    kbSend(0xf0);
    kbSend(0x07);
    break;
  case 0x46:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x7c);
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x12);
    break;
  case 0x47:
    kbSend(0xf0);
    kbSend(0x7e);
    break;
  case 0x48:
    break;
  case 0x49:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x70);
    break;
  case 0x4a:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x6c);
    break;
  case 0x4b:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x7d);
    break;
  case 0x4c:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x71);
    break;
  case 0x4d:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x69);
    break;
  case 0x4e:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x7a);
    break;
  case 0x35:
    kbSend(0xf0);
    kbSend(0x0e);
    break;
  case 0x1e:
    kbSend(0xf0);
    kbSend(0x16);
    break;
  case 0x1f:
    kbSend(0xf0);
    kbSend(0x1e);
    break;
  case 0x20:
    kbSend(0xf0);
    kbSend(0x26);
    break;
  case 0x21:
    kbSend(0xf0);
    kbSend(0x25);
    break;
  case 0x22:
    kbSend(0xf0);
    kbSend(0x2e);
    break;
  case 0x23:
    kbSend(0xf0);
    kbSend(0x36);
    break;
  case 0x24:
    kbSend(0xf0);
    kbSend(0x3d);
    break;
  case 0x25:
    kbSend(0xf0);
    kbSend(0x3e);
    break;
  case 0x26:
    kbSend(0xf0);
    kbSend(0x46);
    break;
  case 0x27:
    kbSend(0xf0);
    kbSend(0x45);
    break;
  case 0x2d:
    kbSend(0xf0);
    kbSend(0x4e);
    break;
  case 0x2e:
    kbSend(0xf0);
    kbSend(0x55);
    break;
  case 0x2a:
    kbSend(0xf0);
    kbSend(0x66);
    break;
  case 0x2b:
    kbSend(0xf0);
    kbSend(0x0d);
    break;
  case 0x14:
    kbSend(0xf0);
    kbSend(0x15);
    break;
  case 0x1a:
    kbSend(0xf0);
    kbSend(0x1d);
    break;
  case 0x08:
    kbSend(0xf0);
    kbSend(0x24);
    break;
  case 0x15:
    kbSend(0xf0);
    kbSend(0x2d);
    break;
  case 0x17:
    kbSend(0xf0);
    kbSend(0x2c);
    break;
  case 0x1c:
    kbSend(0xf0);
    kbSend(0x35);
    break;
  case 0x18:
    kbSend(0xf0);
    kbSend(0x3c);
    break;
  case 0x0c:
    kbSend(0xf0);
    kbSend(0x43);
    break;
  case 0x12:
    kbSend(0xf0);
    kbSend(0x44);
    break;
  case 0x13:
    kbSend(0xf0);
    kbSend(0x4d);
    break;
  case 0x2f:
    kbSend(0xf0);
    kbSend(0x54);
    break;
  case 0x30:
    kbSend(0xf0);
    kbSend(0x5b);
    break;
  case 0x31:
    kbSend(0xf0);
    kbSend(0x5d);
    break;
  case 0x39:
    kbSend(0xf0);
    kbSend(0x58);
    break;
  case 0x04:
    kbSend(0xf0);
    kbSend(0x1c);
    break;
  case 0x16:
    kbSend(0xf0);
    kbSend(0x1b);
    break;
  case 0x07:
    kbSend(0xf0);
    kbSend(0x23);
    break;
  case 0x09:
    kbSend(0xf0);
    kbSend(0x2b);
    break;
  case 0x0a:
    kbSend(0xf0);
    kbSend(0x34);
    break;
  case 0x0b:
    kbSend(0xf0);
    kbSend(0x33);
    break;
  case 0x0d:
    kbSend(0xf0);
    kbSend(0x3b);
    break;
  case 0x0e:
    kbSend(0xf0);
    kbSend(0x42);
    break;
  case 0x0f:
    kbSend(0xf0);
    kbSend(0x4b);
    break;
  case 0x33:
    kbSend(0xf0);
    kbSend(0x4c);
    break;
  case 0x34:
    kbSend(0xf0);
    kbSend(0x52);
    break;
  case 0x28:
    kbSend(0xf0);
    kbSend(0x5a);
    break;
    break;
  case 0x1d:
    kbSend(0xf0);
    kbSend(0x1a);
    break;
  case 0x1b:
    kbSend(0xf0);
    kbSend(0x22);
    break;
  case 0x06:
    kbSend(0xf0);
    kbSend(0x21);
    break;
  case 0x19:
    kbSend(0xf0);
    kbSend(0x2a);
    break;
  case 0x05:
    kbSend(0xf0);
    kbSend(0x32);
    break;
  case 0x11:
    kbSend(0xf0);
    kbSend(0x31);
    break;
  case 0x10:
    kbSend(0xf0);
    kbSend(0x3a);
    break;
  case 0x36:
    kbSend(0xf0);
    kbSend(0x41);
    break;
  case 0x37:
    kbSend(0xf0);
    kbSend(0x49);
    break;
  case 0x38:
    kbSend(0xf0);
    kbSend(0x4a);
  case 0x52:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x75);
    break;
  case 0x50:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x6b);
    break;
  case 0x51:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x72);
    break;
  case 0x4f:
    kbSend(0xe0);
    kbSend(0xf0);
    kbSend(0x74);

  }
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  switch(key) {
  case 0x29:
    kbSend(0x76);
    break;
  case 0x3a:
    kbSend(0x05);
    break;
  case 0x3b:
    kbSend(0x06);
    break;
  case 0x3c:
    kbSend(0x04);
    break;
  case 0x3d:
    kbSend(0x0c);
    break;
  case 0x3e:
    kbSend(0x03);
    break;
  case 0x3f:
    kbSend(0x0b);
    break;
  case 0x40:
    kbSend(0x83);
    break;
  case 0x41:
    kbSend(0x0a);
    break;
  case 0x42:
    kbSend(0x01);
    break;
  case 0x43:
    kbSend(0x09);
    break;
  case 0x44:
    kbSend(0x78);
    break;
  case 0x45:
    kbSend(0x07);
    break;
  case 0x46:
    kbSend(0xe0);
    kbSend(0x12);
    kbSend(0xe0);
    kbSend(0x7c);
    break;
  case 0x47:
    kbSend(0x7e);
    break;
  case 0x48:
    kbSend(0xe1);
    kbSend(0x14);
    kbSend(0x77);
    kbSend(0xe1);
    kbSend(0xf0);
    kbSend(0x14);
    kbSend(0xe0);
    kbSend(0x77);
    break;
  case 0x49:
    kbSend(0xe0);
    kbSend(0x70);
    break;
  case 0x4a:
    kbSend(0xe0);
    kbSend(0x6c);
    break;
  case 0x4b:
    kbSend(0xe0);
    kbSend(0x7d);
    break;
  case 0x4c:
    kbSend(0xe0);
    kbSend(0x71);
    break;
  case 0x4d:
    kbSend(0xe0);
    kbSend(0x69);
    break;
  case 0x4e:
    kbSend(0xe0);
    kbSend(0x7a);
    break;
  case 0x35:
    kbSend(0x0e);
    break;
  case 0x1e:
    kbSend(0x16);
    break;
  case 0x1f:
    kbSend(0x1e);
    break;
  case 0x20:
    kbSend(0x26);
    break;
  case 0x21:
    kbSend(0x25);
    break;
  case 0x22:
    kbSend(0x2e);
    break;
  case 0x23:
    kbSend(0x36);
    break;
  case 0x24:
    kbSend(0x3d);
    break;
  case 0x25:
    kbSend(0x3e);
    break;
  case 0x26:
    kbSend(0x46);
    break;
  case 0x27:
    kbSend(0x45);
    break;
  case 0x2d:
    kbSend(0x4e);
    break;
  case 0x2e:
    kbSend(0x55);
    break;
  case 0x2a:
    kbSend(0x66);
    break;
  case 0x2b:
    kbSend(0x0d);
    break;
  case 0x14:
    kbSend(0x15);
    break;
  case 0x1a:
    kbSend(0x1d);
    break;
  case 0x08:
    kbSend(0x24);
    break;
  case 0x15:
    kbSend(0x2d);
    break;
  case 0x17:
    kbSend(0x2c);
    break;
  case 0x1c:
    kbSend(0x35);
    break;
  case 0x18:
    kbSend(0x3c);
    break;
  case 0x0c:
    kbSend(0x43);
    break;
  case 0x12:
    kbSend(0x44);
    break;
  case 0x13:
    kbSend(0x4d);
    break;
  case 0x2f:
    kbSend(0x54);
    break;
  case 0x30:
    kbSend(0x5b);
    break;
  case 0x31:
    kbSend(0x5d);
    break;
  case 0x39:
    kbSend(0x58);
    break;
  case 0x04:
    kbSend(0x1c);
    break;
  case 0x16:
    kbSend(0x1b);
    break;
  case 0x07:
    kbSend(0x23);
    break;
  case 0x09:
    kbSend(0x2b);
    break;
  case 0x0a:
    kbSend(0x34);
    break;
  case 0x0b:
    kbSend(0x33);
    break;
  case 0x0d:
    kbSend(0x3b);
    break;
  case 0x0e:
    kbSend(0x42);
    break;
  case 0x0f:
    kbSend(0x4b);
    break;
  case 0x33:
    kbSend(0x4c);
    break;
  case 0x34:
    kbSend(0x52);
    break;
  case 0x28:
    kbSend(0x5a);
    break;
  case 0x1d:
    kbSend(0x1a);
    break;
  case 0x1b:
    kbSend(0x22);
    break;
  case 0x06:
    kbSend(0x21);
    break;
  case 0x19:
    kbSend(0x2a);
    break;
  case 0x05:
    kbSend(0x32);
    break;
  case 0x11:
    kbSend(0x31);
    break;
  case 0x10:
    kbSend(0x3a);
    break;
  case 0x36:
    kbSend(0x41);
    break;
  case 0x37:
    kbSend(0x49);
    break;
  case 0x38:
    kbSend(0x4a);
    break;
  case 0x52:
    kbSend(0xe0);
    kbSend(0x75);
    break;
  case 0x50:
    kbSend(0xe0);
    kbSend(0x6b);
    break;
  case 0x51:
    kbSend(0xe0);
    kbSend(0x72);
    break;
  case 0x4f:
    kbSend(0xe0);
    kbSend(0x74);
    break;
  }
}

void ack() {
  //acknowledge commands
  while(keyboard.write(0xFA));
}

int kbdCmd(int command) {
  unsigned char val;
  Serial.print("Rcv: 0x");
  Serial.println(command, HEX);
  switch (command) {
  case 0xFF: //reset
    ack();
    //the while loop lets us wait for the host to be ready
    while(keyboard.write(0xAA)!=0);
    break;
  case 0xFE: //resend
    ack();
    break;
  case 0xF6: //set defaults
    //enter stream mode
    ack();
    break;
  case 0xF5: //disable data reporting
    //FM
    enabled = 0;
    ack();
    break;
  case 0xF4: //enable data reporting
    //FM
    enabled = 1;
    ack();
    break;
  case 0xF3: //set typematic rate
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  case 0xF2: //get device id
    ack();
    keyboard.write(0xAB);
    keyboard.write(0x83);
    break;
  case 0xF0: //set scan code set
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  case 0xEE: //echo
    //ack();
    keyboard.write(0xEE);
    break;
  case 0xED: //set/reset LEDs
    ack();
    keyboard.read(&val); //do nothing with the rate
    ack();
    break;
  }
}

void connectHost() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay(200);

  HidKeyboard.SetReportParser(0, &Prs);

  pinMode(LedPin, OUTPUT);

  keyboard.write(0xAA);
  Serial.println("Output Connected");
  delay(100);

  connectHost();
  Serial.println("\nSerial Host Connected");
  Serial.flush();
}

void loop() {
  if( (digitalRead(ClkPin)==LOW) || (digitalRead(DataPin) == LOW)) {
    if(digitalRead(ClkPin)==LOW){
      Serial.println("Clock is LOW");
    } else {
      Serial.println("Data is LOW");
    }
    unsigned char c;
    while(keyboard.read(&c));
    kbdCmd(c);
    Serial.print("Target: 0x");
    Serial.println(c, HEX);
  }

  Usb.Task();
}

