#include <Convert.h>
#include <Bounce2.h>
#include <LiquidCrystal.h>


/**
 * Converting morsecode to characters on display with Arduino
 * School project in course DT223A - Digital Electronic System Design
 */
  LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
  Convert convert = Convert();
  
  const int signalButtonPin(2);
  const int sendButtonPin(3);
  const int scrollRightPin(8);
  const int scrollLeftPin(9);

  Bounce signalButton = Bounce(); 
  Bounce sendButton = Bounce();
  Bounce scrollRightButton = Bounce();
  Bounce scrollLeftButton = Bounce();

  unsigned long pressedTime;
  unsigned long noSignalStartTime;

  String charSignal = "";
  int charCounter = 0;
  int message[180];

void setup() {
  lcd.begin(16, 2);
  
  pinMode(signalButtonPin, INPUT);
  pinMode(sendButtonPin, INPUT);
  pinMode(scrollRightPin, INPUT);
  pinMode(scrollLeftPin, INPUT);
  
  signalButton.attach(signalButtonPin);
  signalButton.interval(1);

  sendButton.attach(sendButtonPin);
  sendButton.interval(5);

  scrollRightButton.attach(scrollRightPin);
  scrollRightButton.interval(5);

  scrollLeftButton.attach(scrollLeftPin);
  scrollLeftButton.interval(5);

  Serial.begin(9600);
  Serial.println("Morsecode to character converter now running.");
  lcd.print("Morse -> Char");
  
  attachInterrupt(digitalPinToInterrupt(sendButtonPin), displayMessage, RISING);
}

void loop() {
  sendButton.update();
  signalButton.update();
  scrollLeftButton.update();
  scrollRightButton.update();
  
  detectSignalTime();

  if(charSignal.length() == 10){
    saveCurrentChar();
  }
  
  if(pressedTime > 30 && pressedTime < 600){
    charSignal += "01";
    Serial.print("Short   | ");
    Serial.print(pressedTime);
    Serial.println(" ms");
    pressedTime = 0;
  }else if(pressedTime > 1000 && pressedTime < 2000){
    charSignal += "10";
    Serial.print("Long    | ");
    Serial.print(pressedTime);
    Serial.println(" ms");
    pressedTime = 0;
  }

  if(scrollLeftButton.read() == 1){
    while(scrollLeftButton.read() == 1){
      scrollLeftButton.update();
    }
    scrollLeft();
  }
  
  if(scrollRightButton.read() == 1){
    while(scrollRightButton.read() == 1){
      scrollRightButton.update();
    }
    scrollRight();
  }
}

void displayMessage(){
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 if (interrupt_time - last_interrupt_time > 200) 
 {
    String firstRow = "";
    if(charSignal != ""){
      saveCurrentChar();
    }
    lcd.clear();
    Serial.println("Message: ");
    Serial.println("Char | int value | binary signal");
    for(int i = 0; i < charCounter; i++){
      int currentChar = message[i];
      Serial.print(codeToCharacter(convert.decimalToBinary(currentChar)));
      Serial.print(" | ");
      Serial.print(currentChar);
      Serial.print(" | ");
      Serial.println(currentChar, BIN);
      firstRow += codeToCharacter(convert.decimalToBinary(currentChar));
    }
    lcd.print(firstRow);
    charCounter = 0;
 }
 last_interrupt_time = interrupt_time;
}

String codeToCharacter(String value){
  value = addZeroToBinary(value);
  if(value == "0110"){
    return "A";
  }else if(value == "10010101"){
    return "B";
  }else if(value == "10011001"){
    return "C";
  }else if(value == "100101"){
    return "D";
  }else if(value == "01"){
    return "E";
  }else if(value == "01011001"){
    return "F";
  }else if(value == "101001"){
    return "G";
  }else if(value == "01010101"){
    return "H";
  }else if(value == "0101"){
    return "I";
  }else if(value == "01101010"){
    return "J";
  }else if(value == "100110"){
    return "K";
  }else if(value == "01100101"){
    return "L";
  }else if(value == "1010"){
    return "M";
  }else if(value == "1001"){
    return "N";
  }else if(value == "101010"){
    return "O";
  }else if(value == "01101001"){
    return "P";
  }else if(value == "10100110"){
    return "Q";
  }else if(value == "011001"){
    return "R";
  }else if(value == "010101"){
    return "S";
  }else if(value == "10"){
    return "T";
  }else if(value == "010110"){
    return "U";
  }else if(value == "01010110"){
    return "V";
  }else if(value == "011010"){
    return "W";
  }else if(value == "10010110"){
    return "X";
  }else if(value == "10011010"){
    return "Y";
  }else if(value == "10100101"){
    return "Z";
  }else if(value == "0110101010"){
    return "1";
  }else if(value == "0101101010"){
    return "2";
  }else if(value == "0101011010"){
    return "3";
  }else if(value == "0101010110"){
    return "4";
  }else if(value == "0101010101"){
    return "5";
  }else if(value == "1001010101"){
    return "6";
  }else if(value == "1010010101"){
    return "7";
  }else if(value == "1010100101"){
    return "8";
  }else if(value == "1010101001"){
    return "9";
  }else if(value == "1010101010"){
    return "0";
  }else if(value == "0"){
    return " ";
  }else{
    return "*";
  }
}

void addSpaceToMessage(){
  Serial.println("Storing | space");
  message[charCounter] = 0;
  charCounter++;
}

void saveCurrentChar(){
  if(charSignal != "" && charCounter < 180){
    Serial.print("Storing | ");
    Serial.println(codeToCharacter(charSignal));
    message[charCounter] = convert.binaryToDecimal(charSignal);
    charSignal = "";
    charCounter++;
  }
}

void detectSignalTime(){
  unsigned long totalNoSignalTime;
  unsigned long startTime;
  if(signalButton.read() == 1){
    totalNoSignalTime = millis() - noSignalStartTime;

    if((totalNoSignalTime > 2000 && totalNoSignalTime < 4000) && noSignalStartTime != 0){
      saveCurrentChar();
    }else if(totalNoSignalTime > 7000 && noSignalStartTime != 0){
      saveCurrentChar();
      addSpaceToMessage();
    }

    startTime = millis();
    while(signalButton.read() == 1){
      signalButton.update();
    }
    pressedTime = millis() - startTime;
    noSignalStartTime = millis();
  }
}

void scrollLeft(){
  for(int positionCounter = 0; positionCounter < 16; positionCounter++){
    lcd.scrollDisplayLeft();
    delay(150);
  }
}

void scrollRight(){
  for(int positionCounter = 0; positionCounter < 16; positionCounter++){
    lcd.scrollDisplayRight();
    delay(150);
  }
}

String addZeroToBinary(String value){
  int valLength = value.length();
  if((valLength % 2) != 0 && value != "0"){
    return "0" + value;
  }else{
    return value;
  }
}


