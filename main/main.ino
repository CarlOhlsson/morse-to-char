#include <Bounce2.h>

/**
 * Converting morsecode to characters on display with Arduino
 * School project in course DT223A - Digital Electronic System Design
 */

  const int signalButtonPin(2);
  const int sendButtonPin(3);

  Bounce signalButton = Bounce(); 
  Bounce sendButton = Bounce();

  unsigned long pressedTime;
  unsigned long noSignalStartTime;

  int oneTimeUnit = 100;

void setup() {
  pinMode(signalButtonPin, INPUT);
  pinMode(sendButtonPin, INPUT);
  
  signalButton.attach(signalButtonPin);
  signalButton.interval(1);

  sendButton.attach(sendButtonPin);
  sendButton.interval(5);

  Serial.begin(9600);
  Serial.println("Starting program :)");
}

void loop() {
  signalButton.update();
  sendButton.update();
  detectSignalTime();
}

void detectSignalTime(){
  unsigned long startTime;
  if(signalButton.read() == 1){
    startTime = millis();
    while(signalButton.read() == 1){
      signalButton.update();
    }
    pressedTime = millis() - startTime;
    Serial.println(pressedTime);
    noSignalStartTime = millis();
  }
}



