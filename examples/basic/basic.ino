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

  myBLOX.analogWriteS3(PIN_MOSFET1, 100);
  myBLOX.analogWriteS3(PIN_MOSFET1, 50);
  myBLOX.log("Set switched output 1 to 100% and output 2 to 50%");

  if (hasSD()) {logToSD("file.txt", "Hello World");}
  myBLOX.log("Logged test data to SD");

  // Log the run to Serial terminal
  myBLOX.log("Loop ran");
}
