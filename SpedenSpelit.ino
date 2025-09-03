#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile unsigned long lastPressTime = 0;
volatile bool buttonPressed = false;
volatile bool buttonFlag=false;
int leds[]={1,2,3,4};
int score=0;
void setup()
{
  Serial.begin(9600);
 initButtonsAndButtonInterrupts();
 initializeLeds();

}

void loop()
{



char lightedLed=random(1,4);

setLed(lightedLed);
delay(2000);
clearAllLeds();
delay(200);

  if(buttonNumber>=0)
  {
    pressedButton();
      if(lightedLed == buttonNumber){
        score++;
        Serial.print("sait pisteen");
        Serial.println(score);
      }if(lightedLed != buttonNumber){
      Serial.print("väärin peli ohi");
      while(1);
    }
     // start the game if buttonNumber == 4
     // check the game if 0<=buttonNumber<4
     buttonNumber=-1;
  }

  if(newTimerInterrupt == true)
  {
     // new random number must be generated
     // and corresponding let must be activated
  }
}

void initializeTimer(void)
{
	// see requirements for the function from SpedenSpelit.h
}
ISR(TIMER1_COMPA_vect)
{
  /*
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
  
}


void checkGame(byte nbrOfButtonPush)
{
	// see requirements for the function from SpedenSpelit.h
}


void initializeGame()
{
	// see requirements for the function from SpedenSpelit.h
}

void startTheGame()
{
   // see requirements for the function from SpedenSpelit.h
}

