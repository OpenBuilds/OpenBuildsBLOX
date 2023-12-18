#ifndef OpenBuildsBLOX_h
#define OpenBuildsBLOX_h

#include <FlexyStepper.h>
#include <FastLED.h>
#include <ESP32Servo.h>

class OpenBuildsBLOX {
public:
  OpenBuildsBLOX();

  // FlexyStepper functions
  void moveTo(int steps);
  void setSpeed(int speed);

  // FastLED functions
  void setColor(int red, int green, int blue);
  void show();

  // ESP32Servo functions
  void attachServo(int pin);
  void writeServo(int angle);

private:
  FlexyStepper stepper;
  CRGB leds[NUM_LEDS];
  Servo servo;
  void setupI2C();
  void setupSPI();
  
};

// Define popular colors
#define RED CRGB(255, 0, 0)
#define GREEN CRGB(0, 255, 0)
#define BLUE CRGB(0, 0, 255)
#define YELLOW CRGB(255, 255, 0)
#define ORANGE CRGB(255, 165, 0)
#define PURPLE CRGB(128, 0, 128)
#define PINK CRGB(255, 182, 193)
#define CYAN CRGB(0, 255, 255)
#define WHITE CRGB(255, 255, 255)
#define BLACK CRGB(0, 0, 0)
// Add more colors as needed

#endif
