#include <Arduino.h>
//#include <Streaming.h>

#include "MotionSensor.h"

#define PIN_MOTION_SENSOR 6
#define PIN_BUTTON 2

#define PIN_RELAY 8

#define PIN_LED_ON 10
#define PIN_LED_AUTO 11
#define PIN_LED_OFF 12

#define BUTTON_CLICK_DELAY 500
#define MOTION_DETECT_DELAY 500

// 10 min
#define AUTO_LIGHT_TIME 600000

MotionSensor motionSensor(PIN_MOTION_SENSOR);

void setup()
{
  // Serial.begin(115200);
  // Serial << "Setup..." << endl;

  motionSensor.init();

  pinMode(PIN_BUTTON, INPUT_PULLUP);

  pinMode(PIN_LED_ON, OUTPUT);
  pinMode(PIN_LED_AUTO, OUTPUT);
  pinMode(PIN_LED_OFF, OUTPUT);

  pinMode(PIN_RELAY, OUTPUT);


  digitalWrite(PIN_LED_ON, HIGH);
  digitalWrite(PIN_LED_AUTO, HIGH);
  digitalWrite(PIN_LED_OFF, HIGH);
  digitalWrite(PIN_RELAY, HIGH);

  delay(2000);

  digitalWrite(PIN_LED_ON, LOW);
  digitalWrite(PIN_LED_AUTO, LOW);
  digitalWrite(PIN_LED_OFF, LOW);
  digitalWrite(PIN_RELAY, LOW);

  delay(2000);

  digitalWrite(PIN_LED_ON, HIGH);
  digitalWrite(PIN_LED_AUTO, HIGH);
  digitalWrite(PIN_LED_OFF, HIGH);
  digitalWrite(PIN_RELAY, HIGH);

  delay(2000);

  digitalWrite(PIN_LED_ON, LOW);
  digitalWrite(PIN_LED_AUTO, HIGH);
  digitalWrite(PIN_LED_OFF, LOW);
  digitalWrite(PIN_RELAY, LOW);

  // Serial << "Setup ok" << endl;
}

void loop()
{
  uint32_t now = millis();
  static uint32_t lastButtonClick = 0;
  static uint32_t lastMotionDetected = 0;

  /**
   * 0 - свет выключен
   * 1 - свет включен
   * 2 - автосвет
   */
  static uint8_t workMode = 2;

  // защита от обнуления millis
  if (now < lastButtonClick) {
    lastButtonClick = 0;
  }
  if (now < lastMotionDetected) {
    lastMotionDetected = 0;
  }

  if ((digitalRead(PIN_BUTTON) == LOW)
      && ((now - lastButtonClick) > BUTTON_CLICK_DELAY)) {
    if (workMode == 0) {
      workMode = 1;
      digitalWrite(PIN_LED_ON, HIGH);
      digitalWrite(PIN_LED_AUTO, LOW);
      digitalWrite(PIN_LED_OFF, LOW);
      digitalWrite(PIN_RELAY, HIGH);
    } else if (workMode == 1) {
      workMode = 2;
      digitalWrite(PIN_LED_ON, LOW);
      digitalWrite(PIN_LED_AUTO, HIGH);
      digitalWrite(PIN_LED_OFF, LOW);
      digitalWrite(PIN_RELAY, LOW);
    } else if (workMode == 2) {
      workMode = 0;
      digitalWrite(PIN_LED_ON, LOW);
      digitalWrite(PIN_LED_AUTO, LOW);
      digitalWrite(PIN_LED_OFF, HIGH);
      digitalWrite(PIN_RELAY, LOW);
    }
    lastButtonClick = now;
  }


  bool isMotion = motionSensor.isMotion();

  if (isMotion
      && ((now - lastMotionDetected) > MOTION_DETECT_DELAY)
      && (2 == workMode)) {
    digitalWrite(PIN_RELAY, HIGH);
    lastMotionDetected = now;
  }

  if (!isMotion
      && ((now - lastMotionDetected) > AUTO_LIGHT_TIME)
      && (2 == workMode)) {
    digitalWrite(PIN_RELAY, LOW);
  }
}