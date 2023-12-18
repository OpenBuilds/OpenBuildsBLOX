#include <OpenBuildsBLOX.h>

OpenBuildsBLOX BLOX;

void setup() {
  // Initialize your library and other setup code here
  BLOX.attachServo(5);
  BLOX.setColor(ORANGE); // Using the predefined constant

}

void loop() {
  // Your main loop code here
  BLOX.writeServo(90);
  delay(1000);
  BLOX.writeServo(0);
  delay(1000);
}
