#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

void initializeDisplay(void); // Alustaa/resetoi näytön sekä arduinon lähdöt oikein

void writeByte(uint8_t number, bool last); // Kirjoittaa numerot näytölle

void writeHighAndLowNumber(uint8_t tens,uint8_t ones); // Pilkkoo kympit ja ykköset erillisiin numeroihin

void showResult(byte result); // Pääfunktio, jolla tulos segmenttinäytöille

#endif
