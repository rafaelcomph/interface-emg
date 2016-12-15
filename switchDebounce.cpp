
#include <Arduino.h>
#include "switchDebounce.h"


switchDebounce::switchDebounce(int pin) {
    pinMode(pin, INPUT_PULLUP);
    state = LOW;
}

void switchDebounce::refresh(int pin) {
 boolean readPin=digitalRead(pin);
 
 if(state =! readPin && !travaDebounce){
   lastDebounceTime = millis();
   travaDebounce = HIGH;
  }
  state=debounce(readPin); //estou aqui, pensar em parametrizar o tempo e não usar 50 hardcoded

}

boolean switchDebounce::debounce(boolean reading){

  if ((millis() - lastDebounceTime) > 50) {

    if (reading != buttonState) 
      buttonState = reading;

    travaDebounce = LOW;
  }

  return buttonState;
}
