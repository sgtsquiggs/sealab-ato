#ifndef UNIT_TEST

#include "Arduino.h"

#include <Time.h>
#include <Adafruit_MCP23008.h>

// MCP23008 pins
static const uint8_t pumpPin = 7;
static const uint8_t greenLEDPin = 6;
static const uint8_t yellowLEDPin = 5;
static const uint8_t redLEDPin = 4;
static const uint8_t lowLevelSensorPin = 3;
static const uint8_t highLevelSensorPin = 2;

static const int maximumPumpDurationInSeconds = 600;
static const int minimumTimeBetweenPumpingsInSeconds = 3600;

bool pumpState = LOW;
time_t pumpStartTime = 0;

Adafruit_MCP23008 mcp;

bool pumpHasBeenRunningTooLong();
bool pumpHasNotActivatedInAWhile();
bool bothSensorsAreHigh();
bool bothSensorsAreLow();
bool pumpIsOff();
bool pumpIsOn();
void turnPumpOff();
void turnPumpOn();
void lightLED(uint8_t ledPin);


void setup() {
  mcp.begin();

  adjustTime(minimumTimeBetweenPumpingsInSeconds);

  mcp.pinMode(pumpPin, OUTPUT);
  mcp.pinMode(lowLevelSensorPin, INPUT);
  mcp.pinMode(highLevelSensorPin, INPUT);
  mcp.pinMode(greenLEDPin, OUTPUT);
  mcp.pinMode(yellowLEDPin, OUTPUT);
  mcp.pinMode(redLEDPin, OUTPUT);

  turnPumpOff();

  delay(1000);
}

void loop() {
  // Pump control
  if (pumpHasBeenRunningTooLong()) {
    turnPumpOff();
  } else if (pumpIsOn() && bothSensorsAreLow()) {
    turnPumpOff();
  } else if (pumpHasNotActivatedInAWhile() && pumpIsOff() && bothSensorsAreHigh()) {
    turnPumpOn();
  }

  // LED control
  if (bothSensorsAreLow()) {
    lightLED(greenLEDPin);
  } else if (bothSensorsAreHigh()) {
    lightLED(redLEDPin);
  } else {
    lightLED(yellowLEDPin);
  }

  delay(200);
}

bool pumpHasBeenRunningTooLong() {
  if (pumpState == HIGH) {
    if (pumpStartTime + maximumPumpDurationInSeconds <= now()) {
      return true;
    }
  }
  return false;
}

bool pumpHasNotActivatedInAWhile() {
  if (pumpStartTime + minimumTimeBetweenPumpingsInSeconds <= now()) {
    return true;
  }
  return false;
}

bool bothSensorsAreHigh() {
  if (mcp.digitalRead(lowLevelSensorPin) == HIGH) {
    if (mcp.digitalRead(highLevelSensorPin) == HIGH) {
      return true;
    }
  }
  return false;
}

bool bothSensorsAreLow() {
  if (mcp.digitalRead(lowLevelSensorPin) == LOW) {
    if (mcp.digitalRead(highLevelSensorPin) == LOW) {
      return true;
    }
  }
  return false;
}

bool pumpIsOff() {
  if (pumpState == LOW) {
    return true;
  }
  return false;
}

bool pumpIsOn() {
  if (pumpState == HIGH) {
    return true;
  }
  return false;
}

void turnPumpOff() {
  mcp.digitalWrite(pumpPin, HIGH);
  pumpState = LOW;
}

void turnPumpOn() {
  mcp.digitalWrite(pumpPin, LOW);
  pumpState = HIGH;
  pumpStartTime = now();
}

void lightLED(uint8_t ledPin)
{
  if (ledPin != greenLEDPin) mcp.digitalWrite(greenLEDPin, LOW);
  if (ledPin != yellowLEDPin) mcp.digitalWrite(yellowLEDPin, LOW);
  if (ledPin != redLEDPin) mcp.digitalWrite(redLEDPin, LOW);

  if (pumpIsOn()) {
    mcp.digitalWrite(ledPin, LOW);
    delay(200);
  }

  mcp.digitalWrite(ledPin, HIGH);
}

#endif
