#include "OpenBuildsBLOX.h"

CRGB leds[LED_COUNT];
Servo servo;
volatile bool limitTriggered = false;

Adafruit_MCP4725 dac1;
Adafruit_MCP4725 dac2;

ESP_FlexyStepper stepper_1;
ESP_FlexyStepper stepper_2;

#if ARDUINO_USB_CDC_ON_BOOT
#define HWSerial Serial0
#define USBSerial Serial
#else
#define HWSerial Serial
USBCDC USBSerial;
#endif

OpenBuildsBLOX::OpenBuildsBLOX() {


}

void OpenBuildsBLOX::startUp() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Starting BLOX Firmware:");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);  // GRB ordering is typical
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  FastLED.show();
  Serial.println("Started RGB LEDs");

  Wire.begin(3, 4);
  Serial.println("Started i2c");

  SPI.begin(35, 37, 36);  // IO35 as MOSI, IO37 as MISO, IO36 as SCK
  Serial.println("Started SPI");

  dac1.begin(0x60);
  dac2.begin(0x61);
  dac1.setVoltage(mvToInt(800), false);
  dac2.setVoltage(mvToInt(800), false);
  Serial.println("Started Stepper Motor DACs");

  pinMode(ENABLE_1, OUTPUT); // Set Stepper Driver1 ENABLE pin mode
  pinMode(ENABLE_2, OUTPUT); // Set Stepper Driver2 ENABLE pin mode
  digitalWrite(ENABLE_1, HIGH); // Enable Stepper 1
  digitalWrite(ENABLE_2, HIGH); // Enable Stepper 2
  Serial.println("Started Stepper Motor Enables");

  pinMode(FAULT_2, INPUT);
  pinMode(FAULT_1, INPUT);
  Serial.println("Started Stepper Motor Fault pin Modes");

  stepper_1.connectToPins(STEP_1, DIR_1);
  stepper_2.connectToPins(STEP_2, DIR_2);

  // Defaults
  stepper_1.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_1.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_1.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

  stepper_2.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_2.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_2.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

  stepper_1.startAsService(0);
  stepper_2.startAsService(0);


  Serial.println("Started Stepper Motor Instances");

  servo.setPeriodHertz(50);    // standard 50 hz servo
  servo.attach(PIN_SERVO, 500, 2000); // attaches the servo on pin 47 to the servo object
  Serial.println("Started RC Servo");

  pinMode(PIN_MOSFET1, OUTPUT);
  pinMode(PIN_MOSFET2, OUTPUT);
  Serial.println("Started Switch Outputs pin Modes");
  //
  // // attachInterrupt(digitalPinToInterrupt(LIMIT_SENSOR_1), limitInterrupt, CHANGE);
  // // attachInterrupt(digitalPinToInterrupt(LIMIT_SENSOR_2), limitInterrupt, CHANGE);
  Serial.println("Started Limit Switch Interrupts");
  //
}


// FlexyStepper functions
void OpenBuildsBLOX::stepper_1_setStepsPerMillimeter(float motorStepPerMillimeter) {stepper_1.setStepsPerMillimeter(motorStepPerMillimeter);}
void OpenBuildsBLOX::stepper_2_setStepsPerMillimeter(float motorStepPerMillimeter) {stepper_2.setStepsPerMillimeter(motorStepPerMillimeter);}

void OpenBuildsBLOX::stepper_1_setAccelerationInMillimetersPerSecondPerSecond(float accelerationInMillimetersPerSecondPerSecond) {stepper_1.setAccelerationInMillimetersPerSecondPerSecond(accelerationInMillimetersPerSecondPerSecond);}
void OpenBuildsBLOX::stepper_2_setAccelerationInMillimetersPerSecondPerSecond(float accelerationInMillimetersPerSecondPerSecond) {stepper_2.setAccelerationInMillimetersPerSecondPerSecond(accelerationInMillimetersPerSecondPerSecond);}

void OpenBuildsBLOX::stepper_1_setDecelerationInMillimetersPerSecondPerSecond(float decelerationInMillimetersPerSecondPerSecond) {stepper_1.setDecelerationInMillimetersPerSecondPerSecond(decelerationInMillimetersPerSecondPerSecond);}
void OpenBuildsBLOX::stepper_2_setDecelerationInMillimetersPerSecondPerSecond(float decelerationInMillimetersPerSecondPerSecond) {stepper_2.setDecelerationInMillimetersPerSecondPerSecond(decelerationInMillimetersPerSecondPerSecond);}

void OpenBuildsBLOX::stepper_1_setCurrentPositionInMillimeters(float currentPositionInMillimeters) {stepper_1.setCurrentPositionInMillimeters(currentPositionInMillimeters);}
void OpenBuildsBLOX::stepper_2_setCurrentPositionInMillimeters(float currentPositionInMillimeters) {stepper_2.setCurrentPositionInMillimeters(currentPositionInMillimeters);}

void OpenBuildsBLOX::stepper_1_setSpeedInMillimetersPerSecond(float speedInMillimetersPerSecond) {stepper_1.setSpeedInMillimetersPerSecond(speedInMillimetersPerSecond);}
void OpenBuildsBLOX::stepper_2_setSpeedInMillimetersPerSecond(float speedInMillimetersPerSecond) {stepper_2.setSpeedInMillimetersPerSecond(speedInMillimetersPerSecond);}

void OpenBuildsBLOX::stepper_1_moveToHomeInMillimeters(signed char directionTowardHome, float speedInMillimetersPerSecond, long maxDistanceToMoveInMillimeters, int homeLimitSwitchPin) {}
void OpenBuildsBLOX::stepper_2_moveToHomeInMillimeters(signed char directionTowardHome, float speedInMillimetersPerSecond, long maxDistanceToMoveInMillimeters, int homeLimitSwitchPin) {}

void OpenBuildsBLOX::stepper_1_moveRelativeInMillimeters(float distanceToMoveInMillimeters) {
  stepper_1.setTargetPositionRelativeInMillimeters(distanceToMoveInMillimeters);
  while (stepper_1.getDistanceToTargetSigned() != 0)
  {
    Serial.printf("Stepper position: %i\n", stepper_1.getCurrentPositionInSteps());
  }
} // Blocking
void OpenBuildsBLOX::stepper_2_moveRelativeInMillimeters(float distanceToMoveInMillimeters) {
  stepper_2.setTargetPositionRelativeInMillimeters(distanceToMoveInMillimeters);
  while (stepper_1.getDistanceToTargetSigned() != 0)
  {
    Serial.printf("Stepper position: %i\n", stepper_2.getCurrentPositionInSteps());
  }
} // Blocking

void OpenBuildsBLOX::stepper_1_moveToPositionInMillimeters(float absolutePositionToMoveToInMillimeters) {stepper_1.moveToPositionInMillimeters(absolutePositionToMoveToInMillimeters);} // Blocking
void OpenBuildsBLOX::stepper_2_moveToPositionInMillimeters(float absolutePositionToMoveToInMillimeters) {stepper_2.moveToPositionInMillimeters(absolutePositionToMoveToInMillimeters);} // Blocking

void OpenBuildsBLOX::stepper_1_setTargetPositionRelativeInMillimeters(float distanceToMoveInMillimeters) {stepper_1.setTargetPositionRelativeInMillimeters(distanceToMoveInMillimeters);} // Non Blocking
void OpenBuildsBLOX::stepper_2_setTargetPositionRelativeInMillimeters(float distanceToMoveInMillimeters) {stepper_2.setTargetPositionRelativeInMillimeters(distanceToMoveInMillimeters);} // Non Blocking

void OpenBuildsBLOX::stepper_1_setCurrent(float milliAmps) {
  // Calculate the millivolt value based on the given current
   float millivolt = milliAmps / 5.0 / 0.22;

   // Map the millivolt value to the DAC range (0 to 3300 mV to 0 to 4096)
   int dacValue = map(millivolt, 0, 3300, 0, 4096);

   // Set the DAC voltage
   dac1.setVoltage(dacValue, false);
}
void OpenBuildsBLOX::stepper_2_setCurrent(float milliAmps) {
  // Calculate the millivolt value based on the given current
   float millivolt = milliAmps / 5.0 / 0.22;

   // Map the millivolt value to the DAC range (0 to 3300 mV to 0 to 4096)
   int dacValue = map(millivolt, 0, 3300, 0, 4096);

   // Set the DAC voltage
   dac2.setVoltage(dacValue, false);
}

// FastLED functions
void OpenBuildsBLOX::led_setColor(const CRGB& color) {
  leds[0] = color;
  leds[1] = color;
  FastLED.show();
}

void OpenBuildsBLOX::servo_setPosition(int angle) {
  servo.write(angle);
}

int OpenBuildsBLOX::mvToInt(int millivolt) {
  int val = map(millivolt, 0, 3300, 0, 4096);
  return val;
}

void OpenBuildsBLOX::limitInterrupt() {
  // This function will be called when either limit sensor changes state (rising or falling edge)
  if (digitalRead(LIMIT_SENSOR_1) == LOW || digitalRead(LIMIT_SENSOR_2) == LOW) {
    // The limit sensor is LOW (falling edge), set the flag
    limitTriggered = true;
  } else {
    // The limit sensor is HIGH (rising edge), clear the flag
    limitTriggered = false;
  }
}

void OpenBuildsBLOX::log(int value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(unsigned int value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(long value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(unsigned long value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(float value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(double value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(char value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(const char* value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(const String& value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(bool value) {
  Serial.println(value ? "true" : "false");
}
