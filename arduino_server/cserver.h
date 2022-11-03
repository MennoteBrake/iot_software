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

struct response handleRequest(struct stream);

// int readyToRespond(enum parserState, const struct
// request*,
//                   struct response*);

struct response makeResponse(struct request* request, int messageBody);

int setupBuffers();
void addSensor1Measurement(int dataSensor1);
void addSensor2Measurement(int dataSensor2);

uint8_t isABufferFull();
int getSensorModus();
int resetBuffers();
#endif
