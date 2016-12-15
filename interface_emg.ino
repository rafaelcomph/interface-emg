#include <Arduino.h>
#include "switchDebounce.h"

#define atuador1        5
#define atuador2        6
#define pinSensor       A3
#define pinButton       9
#define pinLed          10
#define pinBluetooth    4
#define offsetPin       A1


int v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0, v7 = 0, v8 = 0, v9 = 0, v10 = 0;
float valor1 = 0, valor2 = 1000, valor3 = 0;
float valor;
int i = 0, i2 = 0;
int maxValue = 1024; 
boolean trava = false, ledState = false, travaAtuador = true;
double lastMillis = 0;
double quiescentValue = 0;
float offsetValue;

switchDebounce *swdButton;

void calibration() {
  maxValue = 0;
  while (swdButton->state) {
    quiescentValue += analogReadAverage(pinSensor);
    i++;
    digitalWrite(pinLed, ledState);
    ledState = !ledState;
    swdButton->refresh(pinButton);
    delay(50);
  }
  
  while(!swdButton->state){
    swdButton->refresh(pinButton);
  }
  
  while (swdButton->state) {
    Serial.println(maxValue);
    maxValue = max(analogReadAverage(pinSensor), maxValue);
    digitalWrite(pinLed, ledState);   
    swdButton->refresh(pinButton);
    if(millis() - lastMillis > 500){
      lastMillis = millis();
      ledState = !ledState;
    }
  }
  ledState = false;
  digitalWrite(pinLed, ledState);
  quiescentValue = quiescentValue / i;
  travaAtuador = false;
  i = 0;

}

void trigger() {
  if(!travaAtuador){  
    digitalWrite(pinBluetooth, HIGH);
    delay(500);
    digitalWrite(pinBluetooth, LOW);
  }
}

int analogReadAverage(int analogPin) {
  //media de 22 medidas
  int value = 0;
  value = analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value = value >> 3;

  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value = value >> 3;

  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value += analogRead(analogPin); delayMicroseconds(100);
  value = value >> 3;

  return value;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(pinSensor, INPUT);
  pinMode(atuador1, OUTPUT);
  pinMode(atuador2, OUTPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(offsetPin, INPUT);
  pinMode(pinBluetooth, OUTPUT);
  swdButton = new switchDebounce(pinButton);
  delay(1500);
  digitalWrite(pinLed, travaAtuador);
}


void loop() {
  // put your main code here, to run repeatedly:
  swdButton->refresh(pinButton);

  v5 = v4;
  v4 = v3;
  v3 = v2;
  v2 = v1;
  v1 = analogReadAverage(pinSensor);

  
  if (!swdButton->state) {
    lastMillis = millis();
    while (!swdButton->state) {
      swdButton->refresh(pinButton);
    }
    if (millis() - lastMillis > 3000) {
      calibration();
    }
    if (millis() - lastMillis < 3000) {
      travaAtuador = !travaAtuador;
      digitalWrite(pinLed, travaAtuador);
    }
  }

  if ((v1 - quiescentValue) > ((maxValue-quiescentValue)*(offsetValue/1023))) {
    if (!trava) {
      //Serial.println("Atua");
      //Serial.println(i);
      i++;
      trava = true;
      trigger();
    }
  }
  if ((v1 - quiescentValue) < ((maxValue-quiescentValue)*(offsetValue/1023 - 0.15))) {
    trava = false;

  }
  delay(20);
}
