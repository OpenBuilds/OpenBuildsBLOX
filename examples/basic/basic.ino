#include <OpenBuildsBLOX.h>
OpenBuildsBLOX myBLOX;

const char * siren = "siren:d=8,o=5,b=100:d,e,d,e,d,e,d,e";



void setup() {
  // Initialise the BLOX Hardware devices
  myBLOX.startUp();
}

void loop() {

  myBLOX.led_setColor(RED, RED);
  myBLOX.servo_setPosition(180);
  myBLOX.log("LED to RED, Servo to 180deg");

  delay(1000);

  myBLOX.led_setColor(GREEN, GREEN);
  myBLOX.servo_setPosition(0);
  myBLOX.log("LED to RED, Servo to 180deg");

  delay(1000);

  // Stepper Configuration
  myBLOX.stepper_1_setCurrent(700); // milliAmps
  myBLOX.stepper_1_setStepsPerMillimeter(400); // stepsPerMM
  myBLOX.stepper_1_setSpeedInMillimetersPerSecond(120); // Speed
  myBLOX.stepper_1_setAccelerationInMillimetersPerSecondPerSecond(60); // Acceleration
  myBLOX.stepper_1_setDecelerationInMillimetersPerSecondPerSecond(60); // Deceleration

  myBLOX.stepper_2_setCurrent(700); // milliAmps
  myBLOX.stepper_2_setStepsPerMillimeter(400); // stepsPerMM
  myBLOX.stepper_2_setSpeedInMillimetersPerSecond(120); // Speed
  myBLOX.stepper_2_setAccelerationInMillimetersPerSecondPerSecond(60); // Acceleration
  myBLOX.stepper_2_setDecelerationInMillimetersPerSecondPerSecond(60); // Deceleration
  myBLOX.log("Steppers Configured");

  // Home Steppers
  //myBLOX.stepper_1_moveToHomeInMillimeters(1, 20, 5000, 39);
  //myBLOX.stepper_2_moveToHomeInMillimeters(1, 20, 5000, 40);
  //myBLOX.log("Steppers Homed");

  // Absolute Move
  myBLOX.led_setColor(BLUE, RED);
  myBLOX.stepper_1_moveToPositionInMillimeters(32);
  myBLOX.stepper_2_moveToPositionInMillimeters(32);
  myBLOX.stepper_both_moveToPositionInMillimeters(32, 32);
  myBLOX.log("Steppers: Absolute Move");
  delay(100);

  // Relative Move
  myBLOX.led_setColor(YELLOW, GREEN);
  myBLOX.stepper_1_moveRelativeInMillimeters(-32);
  myBLOX.stepper_2_moveRelativeInMillimeters(-32);
  myBLOX.stepper_both_moveRelativeInMillimeters(-32, -32);
  myBLOX.log("Steppers: Relative Move");
  delay(1000);

  // Play melody
  myBLOX.led_setColor(PURPLE, ORANGE);
  myBLOX.playRTTTL(siren);
  myBLOX.log("Played siren test sound");
  delay(1000);

  myBLOX.analogWriteS3(PIN_MOSFET1, 100);
  myBLOX.analogWriteS3(PIN_MOSFET1, 50);
  myBLOX.log("Set switched output 1 to 100% and output 2 to 50%");

  if (hasSD()) {logToSD("file.txt", "Hello World");}
  myBLOX.log("Logged test data to SD");

  // Log the run to Serial terminal
  myBLOX.log("Loop ran");
}
