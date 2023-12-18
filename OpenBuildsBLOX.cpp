#include "OpenBuildsBLOX.h"

OpenBuildsBLOX::OpenBuildsBLOX() {
  // Initialize your library here
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

// ESP32Servo functions
void OpenBuildsBLOX::attachServo(int pin) {
  servo.attach(pin);
}

void OpenBuildsBLOX::writeServo(int angle) {
  servo.write(angle);
}

void OpenBuildsBlox::setupI2C() {
    Wire.begin(3, 4);  // IO3 as SDA, IO4 as SCL
    // Additional I2C setup if needed
}

void OpenBuildsBlox::setupSPI() {
    SPI.begin(35, 37, 36);  // IO35 as MOSI, IO37 as MISO, IO36 as SCK
    // Additional SPI setup if needed
}

bool OpenBuildsBlox::checkLimit(int limitSwitchNumber) {
    if (limitSwitchNumber == 1) {
        return digitalRead(39);
    } else if (limitSwitchNumber == 2) {
        return digitalRead(40);
    }
}
