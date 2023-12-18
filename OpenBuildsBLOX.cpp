#include "OpenBuildsBLOX.h"

OpenBuildsBLOX::OpenBuildsBLOX() {
  // Initialize your library here
  Wire.begin(3, 4);
  SPI.begin(35, 37, 36);  // IO35 as MOSI, IO37 as MISO, IO36 as SCK

  dac1.begin(0x60);
  dac2.begin(0x61);
  dac1.setVoltage(mvToInt(800), false);
  dac2.setVoltage(mvToInt(800), false);

  servo.setPeriodHertz(50);    // standard 50 hz servo
  servo.attach(PIN_SERVO, 500, 2000); // attaches the servo on pin 47 to the servo object

  pinMode(PIN_MOSFET1, OUTPUT);
  pinMode(PIN_MOSFET2, OUTPUT);

  pinMode(ENABLE_1, OUTPUT); // Set Stepper Driver1 ENABLE pin mode
  pinMode(ENABLE_2, OUTPUT); // Set Stepper Driver2 ENABLE pin mode
  digitalWrite(ENABLE_1, HIGH); // Enable Stepper 1
  digitalWrite(ENABLE_2, HIGH); // Enable Stepper 2

  pinMode(FAULT_2, INPUT);
  pinMode(STEP_2, OUTPUT);
  pinMode(DIR_2, OUTPUT);
  digitalWrite(DIR_2, HIGH);

  pinMode(FAULT_1, INPUT);
  pinMode(STEP_1, OUTPUT);
  pinMode(DIR_1, OUTPUT);
  digitalWrite(DIR_1, HIGH);
}

// FlexyStepper functions
void OpenBuildsBLOX::moveTo(int steps) {
  stepper.moveTo(steps);
}

void OpenBuildsBLOX::setSpeed(int speed) {
  stepper.setSpeed(speed);
}

// FastLED functions
void OpenBuildsBLOX::setColor(int red, int green, int blue) {
  fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
}

void OpenBuildsBLOX::show() {
  FastLED.show();
}

void OpenBuildsBLOX::writeServo(int angle) {
  servo.write(angle);
}

bool OpenBuildsBlox::checkLimit(int limitSwitchNumber) {
    if (limitSwitchNumber == 1) {
        return digitalRead(39);
    } else if (limitSwitchNumber == 2) {
        return digitalRead(40);
    }
}

int mvToInt(int millivolt) {
  int val = map(millivolt, 0, 3300, 0, 4096);
  return val;
}
