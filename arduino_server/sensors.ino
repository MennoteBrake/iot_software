extern "C" {
#include "cserver.h"
}

const int sensorPin1 = A0;
const int sensorPin2 = A1;

unsigned long previousMillisSensor;
unsigned int readSpeedSensor;

void setupSensors(const unsigned int readSpeed) {
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  readSpeedSensor = readSpeed;
  previousMillisSensor = 0 - (1000 / readSpeedSensor);
}

int readSensor(const int pinNumber) {
  return analogRead(pinNumber);
}

void readSensors() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisSensor >=
      (1000 / readSpeedSensor)) {
    int dataSensor1 = readSensor(sensorPin1);
    int dataSensor2 = readSensor(sensorPin2);

    addSensor1Measurement(dataSensor1);
    addSensor2Measurement(dataSensor2);

    previousMillisSensor = currentMillis;
  }
}
