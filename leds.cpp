#include "leds.h"


void initializeLeds()
{
// see requirements for this function from leds.h

pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);
pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);

}

void setLed(byte ledNumber)
{
int analogPin[] = {A2,A3,A4,A5};

for(int i = 0; i<4; i++){
  digitalWrite(analogPin[i], LOW);
  }

if (ledNumber >=0 && ledNumber<=3){
  digitalWrite(analogPin[ledNumber], HIGH);
  }
}


void clearAllLeds()
{

 int analogPin[] = {A2,A3,A4,A5};
 for(int i = 0; i<4; i++){
  digitalWrite(analogPin[i], LOW);
  }
}

void setAllLeds()
{

int analogPin[] = {A2,A3,A4,A5};
for(int i = 0; i<4; i++){
  digitalWrite(analogPin[i], HIGH);
  }
}


void show1()
{
// see requirements for this function from leds.h
}

void show2(int rounds)
{
// see requirements for this function from leds.h  
}