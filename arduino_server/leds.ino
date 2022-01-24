const int yellowLed1 = 3;
const int greenLed1 = 4;
const int redLed1 = 6;
const int redLed2 = 5;


void setupLeds(){
  pinMode(yellowLed1, OUTPUT);
  pinMode(greenLed1, OUTPUT);
  pinMode(redLed2, OUTPUT);
  pinMode(redLed1, OUTPUT);
}

void ledsOnErrorMode(){
    digitalWrite(redLed1, HIGH);
    digitalWrite(redLed2, HIGH);
    digitalWrite(greenLed1, HIGH);
    digitalWrite(yellowLed1, HIGH);
}

void ledsBufferFull(){
    digitalWrite(redLed1, HIGH);
    digitalWrite(redLed2, HIGH);
}

void allLedsOff(){
    digitalWrite(redLed1, LOW);
    digitalWrite(redLed2, LOW);
    digitalWrite(greenLed1, LOW);
    digitalWrite(yellowLed1, LOW);
}
