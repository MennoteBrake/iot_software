#ifndef STATISTICS_h
#define STATISTICS_h
#include "circularbuffer.h"

/**
 * @brief Adds the sensor data to the average of that sensor
 * 
 * @param dataSensor1 The measurement data of sensor 1
 * @param dataSensor2 The measurement data of sensor 2
 */
void addToAverage(int dataSensor1, int dataSensor2);

/**
 * @brief Adds data to the average of sensor 1
 * 
 * @param dataSensor1 The measurement data of sensor 1
 */
void addToAverageSensor1(int dataSensor1);

/**
 * @brief Adds data to the average of sensor 2
 * 
 * @param dataSensor2 The measurement data of sensor 2
 */
void addToAverageSensor2(int dataSensor2);

/**
 * @brief Get the Data Average Sensor1 object
 * 
 * @return float 
 */
float getDataAverageSensor1();

/**
 * @brief Get the Data Average Sensor2 object
 * 
 * @return float 
 */
float getDataAverageSensor2();

/**
 * @brief Calculates the stdev
 * 
 * @param sum The sum of the measured values
 * @param sumQuadrant The sum of the quadrants of the measured values
 * @param amountOfMeasurements The amount of measurements 
 * @return float 
 */
float calcStdev(float sum, float sumQuadrant, int amountOfMeasurements);

/**
 * @brief calculates the stdev for sensor 1
 * 
 * @return float 
 */
float calculateStdevSensor1();

/**
 * @brief calculates the stdev for sensor 2
 * 
 * @return float 
 */
float calculateStdevSensor2();

/**
 * @brief Gets the actual value out of the buffer
 * 
 * @param buffer 
 * @return float 
 */
float getActual(cbuffer* buffer);

/**
 * @brief resets the data for sensor 1
 * 
 */
void resetMeasurementsSensor1();

/**
 * @brief resets the data for sensor 2
 * 
 */
void resetMeasurementsSensor2();
#endif