#include "cserver.h"
#include "circularbuffer.h"
#include "statistics.h"
#include <stdio.h>

#define NDEBUG
#define MSG_SIZE 100

int messageBody = 0;
cbuffer* buffer1 = NULL;
cbuffer* buffer2 = NULL;
const int buffer1StartSize = 20;
const int buffer2StartSize = 20;
enum cbmode bufferMode = OVERWRITE_IF_FULL;

const int activeMode = 0;
const int passiveMode = 1;
int dataMode = 0;

struct response handleRequest(struct stream stream) {
  enum parserState state = EXPECT_METHOD;
  struct request request = {.method = METHOD_UNKNOWN, .target = TARGET_UNKNOWN};

  int done = 0;
  char msg[MSG_SIZE] = {0};
  while (!done) {
    struct token token = getNextToken(stream);

#ifndef NDEBUG
    snprintf(msg, MSG_SIZE, "* token type %d, token value (\"%s\")\n", token.type, token.value);
#endif

    enum parserState next = parseNextToken(token, state, &request, &messageBody);

#ifndef NDEBUG
    snprintf(msg, MSG_SIZE, "* parser state %d\n", next);
#endif

    if (state == ERROR || state == DONE) {
      done = DONE;
    }
    state = next;
  }
  return makeResponse(&request, messageBody);
}

struct response makeResponse(struct request* request, int messageBody) {
  struct response response;
  if (request->method == METHOD_UNKNOWN) {
    response.code = BAD_REQUEST_400;
    return response;
  }
  switch (request->target) {
  case TARGET_UNKNOWN:
    response.code = NOT_FOUND_404;
    response.get_actual = 0;
    return response;

  case AVG1:
    response.code = OK_200_GET_AVG;
    response.get_avg = (double)getDataAverageSensor1();
    return response;

  case AVG2:
    response.code = OK_200_GET_AVG;
    response.get_avg = (double)getDataAverageSensor2();
    return response;

  case STDEV1:
    response.code = OK_200_GET_STDEV;
    response.get_stdev = calculateStdevSensor1();
    return response;
    break;

  case STDEV2:
    response.code = OK_200_GET_STDEV;
    response.get_stdev = calculateStdevSensor2();
    return response;
    break;

  case ACTUAL1:
    response.code = OK_200_GET_ACTUAL;
    response.get_actual = getActual(buffer1);
    return response;
    break;

  case ACTUAL2:
    response.code = OK_200_GET_ACTUAL;
    response.get_actual = getActual(buffer2);
    return response;
    break;

  case CONFIG_MODE:
    dataMode = (messageBody == activeMode) ? activeMode : passiveMode;
    response.code = (messageBody == activeMode) ? CREATED_201_PUT_MODE_ACTIVE : CREATED_201_PUT_MODE_PASSIVE;
    return response;

  case CONFIG_BUFF_SIZE:
    cbFree(buffer1);
    buffer1 = cbInit((int8_t)messageBody, bufferMode);
    cbFree(buffer2);
    buffer2 = cbInit((int8_t)messageBody, bufferMode);

    if (buffer1 == NULL || buffer2 == NULL) {
      response.code = INTERNAL_SERVER_ERROR_500;
      return response;
    }

    response.code = CREATED_201_PUT_CBUFFSIZE;
    return response;

  case DEL_SENSOR1:
    while (cbAvailable(buffer1)) {
      cbRead(buffer1);
    }
    resetMeasurementsSensor1();
    response.code = CREATED_201_DELETE_MEASUREMENTS;
    return response;

  case DEL_SENSOR2:
    while (cbAvailable(buffer2)) {
      cbRead(buffer2);
    }
    resetMeasurementsSensor2();
    response.code = CREATED_201_DELETE_MEASUREMENTS;
    return response;

  case POST_SENSOR1:
    addSensor1Measurement(messageBody);
    response.code = CREATED_201_POST_MEASUREMENT;
    return response;

  case POST_SENSOR2:
    addSensor2Measurement(messageBody);
    response.code = CREATED_201_POST_MEASUREMENT;
    return response;

  default:
    response.code = BAD_REQUEST_400;
    return response;
  }
}

int setupBuffers() {
  buffer1 = cbInit(buffer1StartSize, bufferMode);
  buffer2 = cbInit(buffer2StartSize, bufferMode);

  if (buffer1 == NULL || buffer2 == NULL) {
    return 0;
  }
  return 1;
}

void addSensor1Measurement(int dataSensor1) {
  cbAdd(buffer1, (cbtype)dataSensor1);
  addToAverageSensor1(dataSensor1);
}

void addSensor2Measurement(int dataSensor2) {
  cbAdd(buffer2, (cbtype)dataSensor2);
  addToAverageSensor2(dataSensor2);
}

uint8_t isABufferFull() {
  uint8_t temp = 0;
  if (buffer1->count == buffer1->size) {
    ++temp;
  }
  if (buffer2->count == buffer2->size) {
    temp += 2;
  }
  return temp;
}

int getSensorModus() {
  return dataMode;
}

int resetBuffers() {
  cbFree(buffer1);
  cbFree(buffer2);
  resetMeasurementsSensor1();
  resetMeasurementsSensor2();

  return setupBuffers();
}
