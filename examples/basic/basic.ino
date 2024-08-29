/*
  BLOX Example using the OpenBuilds BLOX Arduino Library

  Please use this as a base to get you started, or to test
  your BLOX - you will be extending the code as required
  by your use case but this should give you the basic details
  of how to use our OpenBuilds BLOX Arduino Library with your
  BLOX devices
*/

// Include the Library
#include <OpenBuildsBLOX.h>
OpenBuildsBLOX myBLOX;

// Sample RTTTL melody, used later in the sketch
const char * siren = "siren:d=8,o=5,b=100:d,e,d,e,d,e,d,e";

// Setup: Runs on startup
void setup() {
  // Initialise the BLOX Hardware devices
  myBLOX.startUp();
}

// Loop: Runs over and over again
void loop() {

  // LEDs to RED, Servo to 180deg
  myBLOX.led_setColor(RED, RED);
  myBLOX.servo_setPosition(180);
  myBLOX.log("LED to RED, Servo to 180deg");

  delay(1000);

  // LEDs to GREEN, Servo to 0deg
  myBLOX.led_setColor(GREEN, GREEN);
  myBLOX.servo_setPosition(0);
  myBLOX.log("LED to GREEN, Servo to 0deg");

  delay(1000);

  // Stepper Configuration
  myBLOX.setCurrent(1, 700); // milliAmps
  stepper_1.setStepsPerMillimeter(400); // stepsPerMM
  stepper_1.setSpeedInMillimetersPerSecond(120); // Speed
  stepper_1.setAccelerationInMillimetersPerSecondPerSecond(60); // Acceleration
  stepper_1.setDecelerationInMillimetersPerSecondPerSecond(60); // Deceleration

  myBLOX.setCurrent(2, 700); // milliAmps
  stepper_2.setStepsPerMillimeter(400); // stepsPerMM
  stepper_2.setSpeedInMillimetersPerSecond(120); // Speed
  stepper_2.setAccelerationInMillimetersPerSecondPerSecond(60); // Acceleration
  stepper_2.setDecelerationInMillimetersPerSecondPerSecond(60); // Deceleration
  myBLOX.log("Steppers Configured");

  // Play melody
  myBLOX.led_setColor(PURPLE, ORANGE);
  myBLOX.playRTTTL(siren);
  myBLOX.log("Played siren test sound");
  delay(1000);

  // Set PWM Switched Outputs
  myBLOX.analogWriteS3(PIN_MOSFET1, 100);
  myBLOX.analogWriteS3(PIN_MOSFET1, 50);
  myBLOX.log("Set switched output 1 to 100% and output 2 to 50%");

  // Log something to SD Card
  if (hasSD()) {logToSD("file.txt", "Hello World");}
  myBLOX.log("Logged test data to SD");

  // Log the run to Serial terminal
  myBLOX.log("Loop ran");
}

  
