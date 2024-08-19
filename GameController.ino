#include "ICM_20948.h"
#include <math.h>
#include <Wire.h>
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h"

// Input Pins
const uint8_t Pin_Y = A1;
const uint8_t Pin_G = A0;
const uint8_t Pin_R = A2;
const uint8_t Pin_B = A3;

// Joystick
JOYSTICK joystick;

// Accelerometer
ICM_20948_I2C myICM;

// Output Pins
const uint8_t Pin_LED = LED_BUILTIN;

// Analog Input Range
const int Joystick_Min = 0;
const int Joystick_Max = 1023;  // 10-bit ADC
// ax used

void setup() {
  // Initialization
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);
  myICM.begin(Wire, 1);
  if (joystick.begin() == false) {
    Serial.print("Joystick does not appear to be connected. Please check wiring. Freezing...");
    while (1)
      ;
  }
  // Set input pin modes
  pinMode(Pin_Y, INPUT);
  pinMode(Pin_G, INPUT);
  pinMode(Pin_R, INPUT);
  pinMode(Pin_B, INPUT);
}
void loop() {
  //Read Accel Data
  if (myICM.dataReady()) {
    myICM.getAGMT();
  }
  float ax = myICM.accX() * -1;
  if (abs(ax) < 125) {
    ax = 0;
  }
  boolean pressW = digitalRead(Pin_B);
  boolean pressS = digitalRead(Pin_G);
  boolean pressBoost = digitalRead(Pin_R);
  boolean pressJump = digitalRead(Pin_Y);
  int turn = ax;
  // Set button and trigger states
  // wasd 0000
  int command = 0;
  if (pressW) {
    command += 8;
  }
  if (pressS) {
    command += 2;
  }
  if (turn > 0) {
    command += 1;
  }
  else if (turn < 0) {
    command += 4;
  }
  if (pressBoost) {
    command += 16;
  }
  if (pressJump) {
    command += 32;
  }
  //Horizontal
  if (joystick.getVertical() > 508) {
    command += 64;
  }
  else if (joystick.getVertical() < 508) {
    command += 256;
  }
  //Vertical
  if (joystick.getHorizontal() > 496) {
    command += 512;
  }
  else if (joystick.getHorizontal() < 496) {
    command += 128;
  }
  if (!joystick.getButton()) {
    command += 1024;
  }
  if (command < 10) {
    Serial.print(0);
    Serial.print(0);
    Serial.print(0);
  }
  else if (command < 100) {
    Serial.print(0);
    Serial.print(0);
  }
  else if (command < 1000) {
    Serial.print(0);
  }
  Serial.print(command);
  delay(50);
}