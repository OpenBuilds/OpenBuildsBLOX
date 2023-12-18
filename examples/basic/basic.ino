#include <OpenBuildsBLOX.h>

OpenBuildsBLOX BLOX;

void setup() {
  // Initialize your library and other setup code here
  openBuilds_Blox.attachServo(5);
  openBuilds_Blox.setColor(ORANGE); // Using the predefined constant

}

void loop() {
  // Your main loop code here
  openBuilds_Blox.writeServo(90);
  delay(1000);
  openBuilds_Blox.writeServo(0);
  delay(1000);
}
