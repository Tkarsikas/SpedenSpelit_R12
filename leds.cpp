#include "leds.h"


void initializeLeds() {
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);
pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);
}

void setLed(byte ledNumber) {
digitalWrite(analogPins[ledNumber], HIGH);
}

void clearAllLeds() {
for (int i = 0; i < 4; i++) {
  digitalWrite(analogPins[i], LOW);
  }
}

void setAllLeds() {
for (int i = 0;i < 4;i++) {
  digitalWrite(analogPins[i], HIGH);
  }
}

void show1() {
  for (int i = 0;i < 16; i++) {
    int jaannos = i;
    if (jaannos >= 8 ) {
      jaannos = jaannos - 8;
      setLed(0);
    }
    if (jaannos >= 4) {
      jaannos = jaannos - 4;
      setLed(1);
    }
    if (jaannos >= 2) {
      jaannos = jaannos - 2;
      setLed(2);
    }
    if (jaannos >= 1) {
      setLed(3);
    }
    Serial.print("Luvun numero ");
    Serial.println(i);
    delay(500);
    clearAllLeds();
    delay(500);
  }
  delay(500);
  clearAllLeds();
  delay(500);
}

void show2(int rounds) {
  int delay_time = 600;
    for(int i = 0; i < rounds;i++) {
      for(int j = 3;j >= 0;j--) {
        setLed(j);
        delay(delay_time);
        clearAllLeds();
      }
      delay_time = delay_time * 0.75;
  }
}