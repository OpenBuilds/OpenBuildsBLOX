#include <OpenBuildsBLOX.h>
OpenBuildsBLOX myBLOX;

void setup() {
  // Initialise the BLOX Hardware devices
  myBLOX.startUp();
}

void loop() {
  // LED to RED, Servo to 180deg
  myBLOX.led_setColor(RED);
  myBLOX.servo_setPosition(180);
  delay(1000);

  // LED to GREEN, Servo to 0deg
  myBLOX.led_setColor(GREEN);
  myBLOX.servo_setPosition(0);
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


  // Absolute Move
  myBLOX.led_setColor(BLUE);
  myBLOX.stepper_1_moveToPositionInMillimeters(32);
  myBLOX.stepper_2_moveToPositionInMillimeters(32);
  myBLOX.stepper_both_moveToPositionInMillimeters(32, 32);
  delay(100);

  // Relative Move
  myBLOX.led_setColor(YELLOW);
  myBLOX.stepper_1_moveRelativeInMillimeters(-32);
  myBLOX.stepper_2_moveRelativeInMillimeters(-32);
  myBLOX.stepper_both_moveRelativeInMillimeters(-32, -32);
  delay(1000);

  // Log the run to Serial terminal
  myBLOX.log("Loop ran");
}
