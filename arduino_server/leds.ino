const int yellowLed1 = 5;
const int greenLed1 = 6;
const int redLed1 = 7;
const int redLed2 = 8;
unsigned long interruptStartTime = 0;
unsigned long httpReqStartTime = 0;
const unsigned long interruptLedOnTimeMS = 500;

void setupLeds() {
  pinMode(yellowLed1, OUTPUT);
  pinMode(greenLed1, OUTPUT);
  pinMode(redLed2, OUTPUT);
  pinMode(redLed1, OUTPUT);
}

void ledsOnErrorMode() {
  digitalWrite(redLed1, HIGH);
  digitalWrite(redLed2, HIGH);
  digitalWrite(greenLed1, HIGH);
  digitalWrite(yellowLed1, HIGH);
}

void ledsBufferFull(uint8_t leds) {
  if (leds == 1 || leds == 3) {
    digitalWrite(redLed1, HIGH);
  }
  if (leds == 2 || leds == 3) {
    digitalWrite(redLed2, HIGH);
  }
  if (leds == 0) {
    digitalWrite(redLed1, LOW);
    digitalWrite(redLed2, LOW);
  }
}

void allLedsOff() {
  digitalWrite(redLed1, LOW);
  digitalWrite(redLed2, LOW);
  digitalWrite(greenLed1, LOW);
  digitalWrite(yellowLed1, LOW);
}

void enableInterruptLED() {
  digitalWrite(greenLed1, HIGH);
  interruptStartTime = millis();
}

void disableInterruptLED() {
  if (interruptStartTime != 0 && millis() >= interruptStartTime + interruptLedOnTimeMS) {
    digitalWrite(greenLed1, LOW);
    interruptStartTime = 0;
  }
}

void enableHttpReqLED() {
  digitalWrite(yellowLed1, HIGH);
  httpReqStartTime = millis();
}

void disableHttpReqLED() {
  if (httpReqStartTime != 0 && millis() >= httpReqStartTime + interruptLedOnTimeMS) {
    digitalWrite(yellowLed1, LOW);
    interruptStartTime = 0;
  }
}
