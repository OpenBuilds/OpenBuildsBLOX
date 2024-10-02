#include "OpenBuildsBLOX.h"

CRGB leds[LED_COUNT];

volatile bool limitTriggered = false;

Adafruit_MCP4725 dac1;
Adafruit_MCP4725 dac2;

ESP_FlexyStepper stepper_1;
ESP_FlexyStepper stepper_2;

MelodyPlayer player(BUZZER_PIN, BUZZER_PWMCHANNEL);

#if ARDUINO_USB_CDC_ON_BOOT
#define HWSerial Serial0
#define USBSerial Serial
#else
#define HWSerial Serial
USBCDC USBSerial;
#endif

OpenBuildsBLOX::OpenBuildsBLOX() {
  // Nothing to initialise on instantiation
}

void OpenBuildsBLOX::startUp() {

  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);   // <<<====== solves this issue of delay per https://github.com/espressif/arduino-esp32/issues/6983#issuecomment-1346941805
  Serial.setDebugOutput(true);
  Serial.println("Starting BLOX Firmware:");

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);  // GRB ordering is typical
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  FastLED.show();
  Serial.println("Started RGB LEDs");

  Wire.begin(3, 4);
  Serial.println("Started i2c");

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  Serial.println("Started SPI");

  dac1.begin(0x60);
  dac2.begin(0x61);
  dac1.setVoltage(mvToInt(800), false);
  dac2.setVoltage(mvToInt(800), false);
  Serial.println("Started Stepper Motor DACs");

  pinMode(ENABLE_1, OUTPUT); // Set Stepper Driver1 ENABLE pin mode
  pinMode(ENABLE_2, OUTPUT); // Set Stepper Driver2 ENABLE pin mode
  digitalWrite(ENABLE_1, HIGH); // Enable Stepper 1
  digitalWrite(ENABLE_2, HIGH); // Enable Stepper 2
  Serial.println("Started Stepper Motor Enables");

  pinMode(FAULT_2, INPUT);
  pinMode(FAULT_1, INPUT);
  Serial.println("Started Stepper Motor Fault pin Modes");

  stepper_1.connectToPins(STEP_1, DIR_1);
  stepper_2.connectToPins(STEP_2, DIR_2);

  // Defaults
  stepper_1.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_1.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_1.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

  stepper_2.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper_2.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper_2.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);

  stepper_1.startAsService(0);
  stepper_2.startAsService(0);
  Serial.println("Started Stepper Motor Services");

  pinMode(BUZZER_PIN, OUTPUT);
  // rtttl::begin(BUZZER_PIN, arkanoid);
  Serial.println("Started Piezo Beeper");

  pinMode(PIN_MOSFET1, OUTPUT);
  pinMode(PIN_MOSFET2, OUTPUT);
  Serial.println("Started Switch Outputs pin Modes");

  // Servo needs no init, but adding log for completeness
  Serial.println("Started Servo");

  // SD Card
  if (!SD.begin(SD_CS, SPI, 4000000)) {
    Serial.println("Unable to start SD Card");
  } else {
    uint8_t cardType = SD.cardType();
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    if (cardType == CARD_NONE) {
      Serial.println("No SD card attached");
    } else if (cardType == CARD_MMC) {
      Serial.println("MMC card attached");
    } else if (cardType == CARD_SD) {
      Serial.println("SDSC card attached");
    } else if (cardType == CARD_SDHC) {
      Serial.println("SDHC card attached");
    } else {
      Serial.println("UNKNOWN card");
    }
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.println("Started SD Card");
  }
}


// FlexyStepper functions
void OpenBuildsBLOX::setCurrent(int channel, float milliAmps) {
  // Calculate the millivolt value based on the given current
   float millivolt = milliAmps / 5.0 / 0.22;

   // Map the millivolt value to the DAC range (0 to 3300 mV to 0 to 4096)
   int dacValue = map(millivolt, 0, 3300, 0, 4096);

   // Set the DAC voltage
   if (channel == 1) {
     dac1.setVoltage(dacValue, false);
   } else if (channel == 2) {
    dac2.setVoltage(dacValue, false);
   }
}


// FastLED functions
void OpenBuildsBLOX::led_setColor(const CRGB& color1, const CRGB& color2) {
  leds[0] = color1;
  leds[1] = color2;
  FastLED.show();
}

void OpenBuildsBLOX::servo_setPosition(uint32_t angle) {
  uint8_t channel = SERVO_CHANNEL;
  uint32_t valueMax = 180;
  // Servo pulse width range (in microseconds)
  const uint32_t minUs = 500;  // 0.5ms
  const uint32_t maxUs = 2000;  // 2ms

  // Calculate pulse width in microseconds
  uint32_t pulseWidth = minUs + (angle * (maxUs - minUs) / valueMax);

  // Calculate duty cycle (12-bit resolution, 4096 steps)
  uint32_t duty = (pulseWidth * 4096) / (1000000 / SERVO_BASE_FREQ);

  ledcSetup(SERVO_CHANNEL, SERVO_BASE_FREQ, SERVO_TIMER_12_BIT);
  ledcAttachPin(PIN_SERVO, SERVO_CHANNEL);

  // Write duty to LEDC
  ledcWrite(channel, duty);
  delay(25); // not sure why but doesn't work without a delay after. Unable to find cause or alternative
}

int OpenBuildsBLOX::mvToInt(int millivolt) {
  int val = map(millivolt, 0, 3300, 0, 4096);
  return val;
}

void OpenBuildsBLOX::limitInterrupt() {
  // This function will be called when either limit sensor changes state (rising or falling edge)
  if (digitalRead(LIMIT_SENSOR_1) == LOW || digitalRead(LIMIT_SENSOR_2) == LOW) {
    // The limit sensor is LOW (falling edge), set the flag
    limitTriggered = true;
  } else {
    // The limit sensor is HIGH (rising edge), clear the flag
    limitTriggered = false;
  }
}

void OpenBuildsBLOX::log(int value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(unsigned int value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(long value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(unsigned long value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(float value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(double value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(char value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(const char* value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(const String& value) {
  Serial.println(value);
}

void OpenBuildsBLOX::log(bool value) {
  Serial.println(value ? "true" : "false");
}

void OpenBuildsBLOX::playRTTTL(const char *melodyString) {
  Melody melody = MelodyFactory.loadRtttlString(melodyString);
  //Serial.print(melody.getTitle());
  player.play(melody);
}

void OpenBuildsBLOX::analogWriteS3(int pin, int dutyCyclePercent) {
  // Setup timer and attach timer to the specified pin
   ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
   ledcAttachPin(pin, LEDC_CHANNEL_0);

   // calculate duty, 4095 from 2 ^ 12 - 1
   uint32_t duty = (4095 / 100) * min(dutyCyclePercent, 100);

   // write duty to LEDC
   ledcWrite(LEDC_CHANNEL_0, duty);
}


float OpenBuildsBLOX::measureDist(int trig_pin, int echo_pin, const char* unit) {
    // Initialize GPIO pins
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);

    // Generate a 10us pulse on TRIG_PIN to trigger the sensor
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // Wait for the ECHO_PIN to go high, indicating the start of the echo
    unsigned long start_time = micros();
    while (!digitalRead(echo_pin)) {
        if ((micros() - start_time) > 5000) // Timeout after 5ms
            return -1.0f;
    }

    // Measure the duration of the echo signal
    start_time = micros();
    while (digitalRead(echo_pin)) {
        if ((micros() - start_time) > 50000) // Timeout after 50ms
            return -1.0f;
    }
    unsigned long end_time = micros();
    unsigned long pulse_duration = end_time - start_time;

    // Calculate distance based on the duration of the echo pulse
    float distance = (pulse_duration * SOUND_SPEED) / (2 * 1000000); // Convert us to s and divide by 2 for round trip

    // Convert distance based on unit parameter
    if (strcmp(unit, "mm") == 0) {
        distance *= 1000; // Convert meters to millimeters
    } else if (strcmp(unit, "cm") == 0) {
        distance *= 100; // Convert meters to centimeters
    } else if (strcmp(unit, "in") == 0) {
        distance *= 39.3701; // Convert meters to inches
    } else if (strcmp(unit, "ft") == 0) {
        distance *= 3.28084; // Convert meters to feet
    }

    return distance;
}

bool OpenBuildsBLOX::hasSD() {
  File root = SD.open("/");
  if (root) {
    root.close();
    return true;
  } else {
    return false;
  }
}

bool OpenBuildsBLOX::logToSD(const char* filename, const char* data) {
  File file = SD.open(filename, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return false;
  }

  // Get current time
  struct tm timeinfo;
  bool hasTime = getLocalTime(&timeinfo);

  // Format time or uptime to string
  char timeStr[64];
  if (hasTime) {
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  } else {
    unsigned long secondsSinceBoot = millis() / 1000;
    snprintf(timeStr, sizeof(timeStr), "Uptime: %lu seconds", secondsSinceBoot);
    Serial.println("No time available (yet), logging uptime");
  }

  // Write time/uptime and data to file in CSV format
  file.print(timeStr);
  file.print(", ");
  file.println(data);

  file.close();
  Serial.println("Data logged to SD card");
  return true;
}
