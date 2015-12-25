/**
 * Converting morsecode to characters on display with Arduino
 * School project in course DT223A - Digital Electronic System Design
 */

const int signalButtonPin = 2;
const int sendButtonPin = 3;
unsigned long startTime;
unsigned long lastButtonPressTime;
boolean newSignalDetected = false;
boolean isPressed = false;

/* Debounce variables*/
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;
int signalButtonState;

void setup() {
  pinMode(signalButtonPin, INPUT);

  Serial.begin(9600);
  Serial.println("Starting program :)");
}

void loop() {
  getPressedTime(signalButtonPin);
  if(newSignalDetected == true){
    Serial.print("Signal time: ");
    Serial.println(lastButtonPressTime);
    newSignalDetected = false;
  }
  
  if(digitalRead(sendButtonPin) == HIGH){
    while(digitalRead(sendButtonPin) == HIGH);
    Serial.println("Sending message!");
  }
  
}

/**
 * Will detect if the signalButton is pressed and if so record the duration
 */
void getPressedTime(int signalPin){
  debounceButton(signalPin);
  
  if(isPressed == true){
    startTime = millis();
    while(digitalRead(signalPin) == HIGH);
    
    isPressed = false;
    
    lastButtonPressTime = (millis() - startTime);
    newSignalDetected = true;
  }
}

/**
 * Will debounce the signalButton
 */
void debounceButton(int pin){
  int reading = digitalRead(pin);

  // if the state has changed reset debounce timer
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // When a signal is long enough accept it as a button press
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != signalButtonState) {
      signalButtonState = reading;

      // If the state is HIGH set isPressed to true so it can be detected
      if (signalButtonState == HIGH) {
        isPressed = true;
      }
    }
  }
  lastButtonState = reading;
}

