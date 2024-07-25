// Dependant Libraries:
//
// https://github.com/pkerspe/ESP-FlexyStepper
// https://github.com/fabianoriccardi/melody-player

#ifndef OpenBuildsBLOX_h
#define OpenBuildsBLOX_h

//To disable pragma messages on compile include this Before including FastLED.h
#define FASTLED_INTERNAL
#include <FastLED.h>
#define LED_PIN    48
#define LED_COUNT 255
extern CRGB leds[LED_COUNT];

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


//PWM Channel Allocation
// Mosfets
#define LEDC_CHANNEL_0     0
#define LEDC_TIMER_12_BIT  12
#define LEDC_BASE_FREQ     5000
//PWM Channel Allocation
// Servo
#define SERVO_CHANNEL     1
//PWM Channel Allocation
// Beeper
#define BUZZER_PWMCHANNEL 2

#include <Adafruit_MCP4725.h>
#include <ESP_FlexyStepper.h>
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

extern Adafruit_MCP4725 dac1;
extern Adafruit_MCP4725 dac2;
extern ESP_FlexyStepper stepper_1;
extern ESP_FlexyStepper stepper_2;

const int SPEED_IN_STEPS_PER_SECOND = 25600;
const int ACCELERATION_IN_STEPS_PER_SECOND = 25600;
const int DECELERATION_IN_STEPS_PER_SECOND = 25600;

// RC Servo
#define SERVO_TIMER_12_BIT  12
#define SERVO_BASE_FREQ     50
#define PIN_SERVO          47

// Mosfets
#define PIN_MOSFET1 41
#define PIN_MOSFET2 42

// Limits Inputs
#define LIMIT_SENSOR_1 39 // Pin for the first limit sensor
#define LIMIT_SENSOR_2 40 // Pin for the second limit sensor

// Beeper (RTTTL)
#include <melody_player.h>
#include <melody_factory.h>
#define BUZZER_PIN 7

// Ultrasonic
#define SOUND_SPEED 343.0f // Speed of sound in air in m/s

#include <FS.h>
#include <SD.h>
#include <SPI.h>

#define SPI_MOSI 35
#define SPI_MISO 37
#define SPI_SCK 36

#define SD_CS 5

#include <Wire.h>


class OpenBuildsBLOX {
public:
  OpenBuildsBLOX();

  void startUp();


  // FastLED functions
  void led_setColor(const CRGB& color1, const CRGB& color2);

  // Servo
  void servo_setPosition(uint32_t angle);

  // Stepper Current DACs
  void setCurrent(int channel, float milliAmps);

  // Serial Logging
  void log(int value);
  void log(unsigned int value);
  void log(long value);
  void log(unsigned long value);
  void log(float value);
  void log(double value);
  void log(char value);
  void log(const char* value);
  void log(const String& value);
  void log(bool value);
  // Add more overloads for other data types as needed

  // Piezo Beeper
  void playRTTTL(const char *melody);
  void playMelody(const char *melody);

  // PWM OUTPUT
  void analogWriteS3(int pin, int dutyCycle);

  // Ultrasonic
  float measureDist(int trig_pin, int echo_pin, const char* unit);

  // SD Logging
  bool hasSD();
  bool logToSD(const char* filename, const char* data);

private:
  int mvToInt(int millivolt);
  void limitInterrupt();
};



#endif
