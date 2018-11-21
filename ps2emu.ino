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

void setup() {
  pinMode(LedPin, OUTPUT);
  //establish serial connection with host
  Serial.begin(9600);
  // establish ps/2 connection with target
  Serial.println("Setup");
  while(keyboard.write(0xAA)!=0){
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);
    delay(500);
  }
  Serial.println("Connected");
  delay(100);

  connectHost();
  Serial.println("\nSerial Host Connected");
  Serial.flush();
}

void loop() {
  unsigned char c;
  if( (digitalRead(ClkPin)==LOW) || (digitalRead(DataPin) == LOW)) {
    if(digitalRead(ClkPin)==LOW){
      Serial.println("Clock is LOW");
    } else {
      Serial.println("Data is LOW");
    }
    while(keyboard.read(&c));
    kbdCmd(c);
    Serial.print("Target: 0x");
    Serial.println(c, HEX);
  }
  else {//if host device wants to send a command:
    //echo ASCII code from terminal and write to ps/2
    if(Serial.available() > 0) {
      incomingByte = Serial.read();
      Serial.print("Send: 0x");
      Serial.println(incomingByte, HEX);

      keyboard.write(incomingByte);
      delay(50);
    }
  }
}
