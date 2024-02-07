#include <OpenBuildsBLOX.h>
OpenBuildsBLOX myBLOX;

const char * mario = "mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";


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

  // Home Steppers
  //myBLOX.stepper_1_moveToHomeInMillimeters(1, 20, 5000, 39);
  //myBLOX.stepper_2_moveToHomeInMillimeters(1, 20, 5000, 40);

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

  // Play melody
  myBLOX.led_setColor(PURPLE);
  myBLOX.playRTTTL(mario);
  delay(1000);

  myBLOX.analogWriteS3(PIN_MOSFET1, 100);
  myBLOX.analogWriteS3(PIN_MOSFET1, 50);

  // Log the run to Serial terminal
  myBLOX.log("Loop ran");
}
