#ifndef OpenBuildsBLOX_h
#define OpenBuildsBLOX_h

#include <FlexyStepper.h>
#include <FastLED.h>
// Define popular colors for the LEDs
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

#include <ESP32Servo.h>

#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac1;
Adafruit_MCP4725 dac2;

// Stepper 1
const int DIR_1 = 8;  // Stepper1 DIR
const int STEP_1 = 9; // Stepper1 STEP
const int ENABLE_1 = 10; // Stepper1 ENABLE
const int FAULT_1 = 11; // Stepper1 Fault

// Stepper 2
const int DIR_2 = 12;  // Stepper2 DIR
const int STEP_2 = 13; // Stepper2 STEP
const int ENABLE_2 = 14; // Stepper2 Enable
const int FAULT_2 = 15; // Stepper2 Fault

#define LED_PIN    48
#define LED_COUNT 2

#define PIN_SERVO 47
Servo servo;  // create servo object to control a servo

#define PIN_MOSFET1 41
#define PIN_MOSFET2 42

#define LIMIT_SENSOR_1 39 // Pin for the first limit sensor
#define LIMIT_SENSOR_2 40 // Pin for the second limit sensor


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
  int mvToInt(int millivolt)
};



#endif
