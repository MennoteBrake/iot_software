#ifndef cserver_h
#define cserver_h

#include "parser.h"

enum statuscode {
  INTERNAL_SERVER_ERROR_500,      // failed to malloc cbuffers
  BAD_REQUEST_400,                // bad request
  NOT_FOUND_404,                  // request target not found
  OK_200_GET_AVG,                 // get mean
  OK_200_GET_STDEV,               // get standard deviation
  OK_200_GET_ACTUAL,              // empty cbuffer, get its mean
  CREATED_201_PUT_MODE_ACTIVE,    // start reading sensors
  CREATED_201_PUT_MODE_PASSIVE,   // stop reading sensors
  CREATED_201_PUT_CBUFFSIZE,      // send new cbuffer size
  CREATED_201_POST_MEASUREMENT,   // send a measurement
  CREATED_201_DELETE_MEASUREMENTS // clear collected data
};

struct response {
  enum statuscode code;
  union {
    double get_avg;
    double get_stdev;
    double get_actual;
  };
};

/**
 * @brief Handles the incoming request
 * 
 * @return struct response 
 */
struct response handleRequest(struct stream);

/**
 * @brief creates a response based on the request, and executes what was asked from the server
 * 
 * @param request the request that was given
 * @param messageBody the body of the message
 * @return struct response 
 */
struct response makeResponse(struct request* request, int messageBody);

/**
 * @brief creates the circular buffers
 * 
 * @return int returns 0 when failed, and 1 when successful
 */
int setupBuffers();

/**
 * @brief adds sensor data to the buffer
 * 
 * @param dataSensor1 
 */
void addSensor1Measurement(int dataSensor1);
/**
 * @brief adds sensor data to the buffer
 * 
 * @param dataSensor2 
 */
void addSensor2Measurement(int dataSensor2);

/**
 * @brief checks if a buffer is full
 * 
 * @return uint8_t 
 */
uint8_t isABufferFull();

/**
 * @brief gets the sensor mode
 * 
 * @return int 
 */
int getSensorModus();

/**
 * @brief resets the buffers
 * 
 * @return int returns 0 when failed, and 1 when successful
 */
int resetBuffers();
#endif
