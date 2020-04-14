
#include <Wire.h>

static const uint8_t numbertable[] = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x77, /* a */
  0x7C, /* b */
  0x39, /* C */
  0x5E, /* d */
  0x79, /* E */
  0x71, /* F */
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(0x70);

  //position to the leftmost of the screen
  Wire.write(0x00);
  Wire.write(numbertable[0]);

  //second from the left
  Wire.write(0x01);
  Wire.write(numbertable[1]);

  //semi-colon in the middle of the screen
  Wire.write(0x02);
  Wire.write(0);

  //second from the right
  Wire.write(0x03);
  Wire.write(numbertable[3]);

  //furthest right digit
  Wire.write(0x04);
  Wire.write(numbertable[4]);
  Wire.endTransmission();
  delay(500);
}
