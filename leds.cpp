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
int analogPin[] = {A2,A3,A4,A5};

for (int value =0; value < 16; value++){
  
  for(int led =0; led<4; led++){
    digitalWrite(analogPin[led], (value >> led) & 0x01);
    //delay(50);
  }
  delay(500);
}
}

void show2(int rounds)
{
// see requirements for this function from leds.h 
// see requirements for this function from leds.h 
int analogPin[] = {A2,A3,A4,A5};
int minDelay =0;
int maxDelay =500;
int delayDecrease = (maxDelay/rounds)+50;

while(maxDelay>minDelay){
 for(int led=0;led<4;led++){
  digitalWrite(analogPin[led], HIGH);
  delay(maxDelay);
    digitalWrite(analogPin[led], LOW);
    delay(maxDelay);

}
    maxDelay -=delayDecrease;

 }
}