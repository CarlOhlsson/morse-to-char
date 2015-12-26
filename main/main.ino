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

  String charSignal = "";
  int charCounter = 0;
  int message[180];

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

  if(charSignal.length() == 10){
    saveCurrentChar();
  }
  
  if(pressedTime > 30 && pressedTime < 1000){
    charSignal += "01";
    Serial.println(charSignal);
    Serial.print(pressedTime);
    Serial.println(" - Short");
    pressedTime = 0;
  }else if(pressedTime > 2000 && pressedTime < 4000){
    charSignal += "10";
    Serial.println(charSignal);
    Serial.print(pressedTime);
    Serial.println(" - Long");
    pressedTime = 0;
  }
  
  if(sendButton.read() == 1){
    while(sendButton.read() == 1){
      sendButton.update();
    }

    Serial.println("Message: ");
    for(int i = 0; i < charCounter; i++){
      Serial.println(message[i]);
    }
  }
}

void addSpaceToMessage(){
  message[charCounter] = 0;
  charCounter++;
}
void saveCurrentChar(){
  Serial.print("Save signal: ");
  Serial.print(charSignal);
  Serial.print(" -> ");
  Serial.println(charSignal.toInt());
  message[charCounter] = charSignal.toInt();
  charSignal = "";
  charCounter++;
}

void detectSignalTime(){
  unsigned long totalNoSignalTime;
  unsigned long startTime;
  if(signalButton.read() == 1){
    totalNoSignalTime = millis() - noSignalStartTime;

    if(totalNoSignalTime > 2000 && totalNoSignalTime < 4000){
      Serial.println("New char detected");
      saveCurrentChar();
    }else if(totalNoSignalTime > 7000){
      Serial.println("New word detected");
      addSpaceToMessage();
      saveCurrentChar();
    }

    startTime = millis();
    while(signalButton.read() == 1){
      signalButton.update();
    }
    pressedTime = millis() - startTime;
    //Serial.println(pressedTime);
    noSignalStartTime = millis();
  }
}



