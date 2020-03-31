#include <Arduino.h>
#include <Streaming.h>

void setup()
{
  Serial.begin(115200);

  Serial << "Setup..." << endl;
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  Serial << "Setup ok" << endl;
}

void loop()
{
}