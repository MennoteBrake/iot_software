#include "statistics.h"
#include <math.h>

float dataAverageSensor1 = 0;
float dataAverageSensor2 = 0;
unsigned int amountOfSensorMeasuresSensor1 = 0;
unsigned int amountOfSensorMeasuresSensor2 = 0;
int sumQuadrantSensor1 = 0;
int sumQuadrantSensor2 = 0;

void addToAverage(int dataSensor1, int dataSensor2) {
  dataAverageSensor1 =
      (((dataAverageSensor1 * amountOfSensorMeasuresSensor1) + dataSensor1) / (1 + amountOfSensorMeasuresSensor1));
  dataAverageSensor2 =
      (((dataAverageSensor2 * amountOfSensorMeasuresSensor2) + dataSensor2) / (1 + amountOfSensorMeasuresSensor2));
  ++amountOfSensorMeasuresSensor1;
  ++amountOfSensorMeasuresSensor2;

  sumQuadrantSensor1 += dataSensor1 * dataSensor1;
  sumQuadrantSensor2 += dataSensor2 * dataSensor2;
}

void addToAverageSensor1(int dataSensor1) {
  dataAverageSensor1 =
      (((dataAverageSensor1 * amountOfSensorMeasuresSensor1) + dataSensor1) / (1 + amountOfSensorMeasuresSensor1));
  ++amountOfSensorMeasuresSensor1;
  sumQuadrantSensor1 += dataSensor1 * dataSensor1;
}

void addToAverageSensor2(int dataSensor2) {
  dataAverageSensor2 =
      (((dataAverageSensor2 * amountOfSensorMeasuresSensor2) + dataSensor2) / (1 + amountOfSensorMeasuresSensor2));
  ++amountOfSensorMeasuresSensor2;
  sumQuadrantSensor2 += dataSensor2 * dataSensor2;
}

float getDataAverageSensor1() {
  if (amountOfSensorMeasuresSensor1 == 0)
    return -1;
  int temp = dataAverageSensor1 * 10;
  return (float)temp * 0.1;
}

float getDataAverageSensor2() {
  if (amountOfSensorMeasuresSensor2 == 0)
    return -1;
  int temp = dataAverageSensor2 * 10;
  return (float)temp * 0.1;
}

float calcStdev(float sum, float sumQuadrant, int amountOfMeasurements) {
  float stdev = sqrt((sumQuadrant - sum * sum / amountOfMeasurements) / amountOfMeasurements);
  return (stdev);
}

float calculateStdevSensor1() {
  if (amountOfSensorMeasuresSensor1 == 0) {
    return -1;
  }
  float stdev =
      calcStdev(dataAverageSensor1 * amountOfSensorMeasuresSensor1, sumQuadrantSensor1, amountOfSensorMeasuresSensor1);
  int temp = stdev * 10;

  return (float)temp * 0.1;
}

float calculateStdevSensor2() {
  if (amountOfSensorMeasuresSensor2 == 0) {
    return -1;
  }

  float stdev =
      calcStdev(dataAverageSensor2 * amountOfSensorMeasuresSensor2, sumQuadrantSensor2, amountOfSensorMeasuresSensor2);
  int temp = stdev * 10;

  return (float)temp * 0.1;
}

float getActual(cbuffer* buffer) {
  if (buffer->count == 0)
    return -1;
  float actual = 0;
  for (int i = 0; i < buffer->count; ++i) {
    cbtype val = cbPeekPos(buffer, i);
    actual += val;
    // printf(" i: %i", i);
    // printf(" val: %i \n", val);
  }
  return actual / buffer->count;
}

void resetMeasurementsSensor1() {
  dataAverageSensor1 = 0;
  amountOfSensorMeasuresSensor1 = 0;
  sumQuadrantSensor1 = 0;
}

void resetMeasurementsSensor2() {
  dataAverageSensor2 = 0;
  amountOfSensorMeasuresSensor2 = 0;
  sumQuadrantSensor2 = 0;
}
