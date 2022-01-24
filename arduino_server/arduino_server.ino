#include <Ethernet.h>

extern "C" {
#include "cserver.h"
#include "circularbuffer.h"
}

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 21);
EthernetServer server(80);
EthernetClient httpClient;

// make httpClient methods available as ordinary functions
int clientAvailable() {
  return httpClient.connected() && httpClient.available();
}
char clientRead() { return httpClient.read(); }
char clientPeek() { return httpClient.peek(); }

//moet in de handlerequest ivm unittest
const int buffer1Size = 20;
const int buffer2Size = buffer1Size;
const unsigned int sensorReadSpeed = 10; //10hz = 10 x per sec
cbuffer* buffer1;
cbuffer* buffer2;

void setup() {
  Serial.begin(9600);
  setupLeds();

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  buffer1 = cbInit(buffer1Size, OVERWRITE_IF_FULL);
  buffer2 = cbInit(buffer2Size, OVERWRITE_IF_FULL);

//  cbuffer* buffer1 = cbInit(buffer1Size, OVERWRITE_IF_FULL);
//  cbuffer* buffer2 = cbInit(buffer2Size, OVERWRITE_IF_FULL);
  
  if(!buffer1 || !buffer2){
    ledsOnErrorMode();  
  }  
  setupSensors(sensorReadSpeed, buffer1, buffer2);
}

void loop() {
  httpClient = server.available();
  if (httpClient) {
    Serial.println("new client");

    struct stream stream = {clientAvailable, clientPeek,
                            clientRead};
    struct response response = handleRequest(stream);

    if (response.code == INTERNAL_SERVER_ERROR_500) {
      httpClient.println(
          F("HTTP/1.1 500 Internal Server Error"));
      httpClient.println();
    } else {
      httpClient.println(
          F("HTTP/1.1 500 Internal Server Error"));
      httpClient.println();
    }

    Serial.print("response code: ");
    Serial.println(response.code);

    delay(1);
    httpClient.stop(); // close connection
    Serial.println("client disconnected");
  }
  readSensors();
//  if(cbAvailable(buffer2) == 1){
//    Serial.print("buffer2: ");
//    Serial.println(cbRead(buffer2));
//  }
//  if(cbAvailable(buffer1) == 1){
//    Serial.print("buffer1: ");
//    Serial.println(cbRead(buffer1));
//  }
}
