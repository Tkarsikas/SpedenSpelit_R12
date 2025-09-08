#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#define bufferSize 10

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
int buttonBuffer[bufferSize];

int ledBuffer[bufferSize];
int leds[]={0,1,2,3};
int score=0;
int randomLedNumber;
int bufferIndexButton=0;
int bufferIndexLed=0;

volatile bool newTimerInterrupt = false;  // for timer interrupt handler

volatile long s;
volatile long ss;

void setup()
{
  Serial.begin(9600);
  initializeTimer();
  initButtonsAndButtonInterrupts();
  initializeLeds();
  startTheGame();

}

void loop()
{
buttonNumber=getPressedButton();
  if(buttonNumber>=0)
  {
addToButtonBuffer(buttonNumber);
  }

  if(newTimerInterrupt == true)
  {
     // new random number must be generated
     // and corresponding led must be activated
     newTimerInterrupt=false;
     randomLedNumber=random(0,4);
     setLed(leds[randomLedNumber]);
     addToLedBuffer(randomLedNumber);
  for(int i =0; i<11;i++){
    Serial.println(ledBuffer[i]);
  }
  Serial.println();
  }
}

void addToButtonBuffer(int a){
  buttonBuffer[bufferIndexButton]=a;
  bufferIndexButton++;

  if(bufferIndexButton==bufferSize){
    bufferIndexButton=0;
  }
}

void addToLedBuffer(int a){
  ledBuffer[bufferIndexLed]=a;
  bufferIndexLed++;

  if(bufferIndexLed==bufferSize){
    bufferIndexLed=0;
  }
}

void initializeTimer(void)
{
	// see requirements for the function from SpedenSpelit.h
  byte maski = 0b00001000;
  TCCR1A = 0;
  TCCR1B |= 0b00001011;
  OCR1A = 25000;
 

  s=0;
  ss=0;
}
ISR(TIMER1_COMPA_vect)
{
  /*
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
ss++;
if(ss==30){
  ss=0;
  s++;
  newTimerInterrupt=true;
}if(s==60){
  s=0;
}
 /* Serial.print(s);
  Serial.print(" : ");
  Serial.println(ss);
*/
}


void checkGame(int nbrOfButtonPush)
{
	// see requirements for the function from SpedenSpelit.h  
  //Serial.println(nbrOfButtonPush);
  //Serial.println(randomLedNumber);
      if(randomLedNumber == nbrOfButtonPush){
        score++;
       // Serial.print("sait pisteen");
        //Serial.println(score);

      
      }if(randomLedNumber != nbrOfButtonPush){
      Serial.print("väärin peli ohi");
      while(digitalRead(6)==1){

      }
    
     // start the game if buttonNumber == 4
     // check the game if 0<=buttonNumber<4
    //Serial.println(nbrOfButtonPush);
    
  }
}


void initializeGame()
{
	// see requirements for the function from SpedenSpelit.h
  int min=0;
  int max=4;
 randomLedNumber=random(min,max);

}

void startTheGame()
{
   // see requirements for the function from SpedenSpelit.h
    TIMSK1 |= 0b00000010;
  interrupts();
initializeGame();

}

