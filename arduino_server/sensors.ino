extern "C" {
#include "circularbuffer.h"
}

const int sensorPin1 = A0;
const int sensorPin2 = A1;

unsigned long previousMillisSensor;
unsigned int readSpeedSensor;

cbuffer* bufferSensor1;
cbuffer* bufferSensor2;

float dataAverageSensor1 = 0;
float dataAverageSensor2 =0;
unsigned int amountOfSensorMeasures = 0;

void setupSensors(const unsigned int readSpeed, cbuffer* cbuff1, cbuffer* cbuff2){
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  readSpeedSensor = readSpeed;
  previousMillisSensor = 0 - (1000 / readSpeedSensor);

  bufferSensor1 = cbuff1;
  bufferSensor2 = cbuff2;
  
}

int readSensor(const int pinNumber){
    return analogRead(pinNumber);
}

void readSensors(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisSensor >= (1000/readSpeedSensor)) {
    int dataSensor1 = readSensor(sensorPin1);
    int dataSensor2 = readSensor(sensorPin2);
    cbAdd(bufferSensor1, dataSensor1);
    cbAdd(bufferSensor2, dataSensor2);
    dataAverageSensor1 = (((dataAverageSensor1 * amountOfSensorMeasures) + dataSensor1) / (1 + amountOfSensorMeasures));
    dataAverageSensor2 = (((dataAverageSensor2 * amountOfSensorMeasures) + dataSensor2) / (1 + amountOfSensorMeasures));
    amountOfSensorMeasures ++;

//    Serial.println("data " + String(dataSensor2));
//    Serial.println(dataAverageSensor2);
    previousMillisSensor = currentMillis;
  }
}
