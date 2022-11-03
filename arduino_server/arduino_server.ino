#include <Ethernet.h>
#include <SPI.h>
extern "C" {
#include "cserver.h"
}

byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x6F, 0xAC};

IPAddress ip(192, 168, 1, 21);
EthernetServer server(80);
EthernetClient httpClient;

// make httpClient methods available as ordinary functions

int clientAvailable() {
  return httpClient.connected() && httpClient.available();
}
char clientRead() { return httpClient.read(); }
char clientPeek() { return httpClient.peek(); }

const unsigned int sensorReadSpeed = 10; // 10hz = 10 x per sec
const int interruptPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onInterrupt, RISING);

  setupLeds();

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  if (setupBuffers() == 0) {
    ledsOnErrorMode();
    Serial.println("ledsOnErrorMode ");
  }
  setupSensors(sensorReadSpeed);
}

void loop() {
  httpClient = server.available();
  if (httpClient) {
    Serial.println("new client");

    struct stream stream = {clientAvailable, clientPeek,
                            clientRead};
    struct response response = handleRequest(stream);
    createResponse(response);

    Serial.print("response code: ");
    Serial.println(response.code);

    delay(1);
    httpClient.stop(); // close connection
    Serial.println("client disconnected");
  }

  ledsBufferFull(isABufferFull());
  if (getSensorModus() == 0) // 0 is active mode
  {
    readSensors();
  }
}

void onInterrupt() {
  if (resetBuffers() == 0) {
    ledsOnErrorMode();
  }
}

void createResponse(struct response response) {
  switch (response.code) {
  case INTERNAL_SERVER_ERROR_500:
    httpClient.println(
        F("HTTP/1.1 500 Internal Server Error"));
    httpClient.println();
  case BAD_REQUEST_400:
    httpClient.println(F("HTTP/1.1 400 BAD REQUEST"));
    break;
  case NOT_FOUND_404:
    httpClient.println(F("HTTP/1.1 404 NOT FOUND"));
    break;
  case OK_200_GET_AVG:
    httpClient.print(F("HTTP/1.1 200 OK \r\n\r\n"));
    httpClient.print(response.get_avg);
    httpClient.println();
    Serial.println(response.get_avg);
    break;
  case OK_200_GET_STDEV:
    httpClient.print(F("HTTP/1.1 200 OK \r\n\r\n"));
    httpClient.print(response.get_stdev);
    httpClient.println();
    Serial.println(response.get_stdev);
    break;
  case OK_200_GET_ACTUAL:
    httpClient.print(F("HTTP/1.1 200 OK \r\n\r\n"));
    httpClient.print(response.get_actual);
    httpClient.println();
    Serial.println(response.get_actual);
    break;
  case CREATED_201_PUT_MODE_ACTIVE:
    httpClient.println(F("HTTP/1.1 201 CREATED"));
    break;
  case CREATED_201_PUT_MODE_PASSIVE:
    httpClient.println(F("HTTP/1.1 201 CREATED"));
    break;
  case CREATED_201_PUT_CBUFFSIZE:
    httpClient.println(F("HTTP/1.1 201 CREATED"));
    break;
  case CREATED_201_POST_MEASUREMENT:
    httpClient.println(F("HTTP/1.1 201 CREATED "));
    break;
  case CREATED_201_DELETE_MEASUREMENTS:
    httpClient.println(F("HTTP/1.1 201 CREATED"));
    break;
  default:
    httpClient.println(
        F("HTTP/1.1 500 Internal Server Error"));
    httpClient.println();
  }
}
