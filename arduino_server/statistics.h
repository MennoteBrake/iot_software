#ifndef STATISTICS_h
#define STATISTICS_h
#include "circularbuffer.h"

void addToAverage(int dataSensor1, int dataSensor2);
void addToAverageSensor1(int dataSensor1);
void addToAverageSensor2(int dataSensor2);

float getDataAverageSensor1();
float getDataAverageSensor2();
float calcStdev(float sum, float sumQuadrant, int amountOfMeasurements);
float calculateStdevSensor1();
float calculateStdevSensor2();
float getActual(cbuffer* buffer);
void resetMeasurementsSensor1();
void resetMeasurementsSensor2();
#endif