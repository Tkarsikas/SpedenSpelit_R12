#ifndef LEDS_H
#define LEDS_H
#include <Arduino.h>

const byte analogPins [] = {A2,A3,A4,A5};

void initializeLeds(); // Pinnit A2-A5 ledeille outputeiksi

void setLed(byte ledNumber); // Sytytetään yksittäinen LED väliltä 0...3

void clearAllLeds(void); // Sammuttaa kaikki LED:t väliltä 0...3

void setAllLeds(void); // Sytyttää kaikki LED:t väliltä 0...3

void show1(void); // Näyttää LED valoilla binääriluvut väliltä 0...15. LSB = A5, MSB = A2

void show2(int); // Sytyttää ledejä järejestyksessä 0,1,2,3 nostaen nopeutta joka kierroksella

#endif