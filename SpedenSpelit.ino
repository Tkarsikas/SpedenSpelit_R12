#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#define bufferSize 10

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;           // for buttons interrupt handler
int buttonBuffer[bufferSize]={0};

int ledBuffer[bufferSize]={0};
int leds[]={0,1,2,3};
int score=0;
int randomLedNumber;
int bufferIndexButton=0;
int bufferIndexLed=0;
int compareIndex=0;

volatile bool newTimerInterrupt = false;  // for timer interrupt handler

volatile long s;
volatile long ss;
volatile int time = 10;

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
 
  if(pressedButton>-1 && pressedButton<4)
  {
  buttonNumber=pressedButton;
  pressedButton= -2;
  addToButtonBuffer(buttonNumber);
  checkGame(compareIndex);
  compareIndex+=1;
  if(compareIndex==10){
    compareIndex=0;
  }
  }

  if(newTimerInterrupt == true)
  {
     // new random number must be generated
     // and corresponding led must be activated
     newTimerInterrupt=false;
     randomLedNumber=random(0,4);
     addToLedBuffer(randomLedNumber);
     clearAllLeds();
     delay(30);
     setLed(leds[randomLedNumber]);
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
if(ss==time){
  ss=0;
  s++;
  newTimerInterrupt=true;
}if(s==10){
  s=0;
  time -= (time*0,1);
}
 /* Serial.print(s);
  Serial.print(" : ");
  Serial.println(ss);
*/
}


void checkGame(int index)
{
	// see requirements for the function from SpedenSpelit.h  
  //Serial.println(nbrOfButtonPush);
       
     
      if(ledBuffer[index] == buttonBuffer[index]){
        score++;
       Serial.println(score);
        
     

      
      }if(ledBuffer[index] != buttonBuffer[index]){
           
      
      
      Serial.println("väärin peli ohi");
      Serial.print("pisteet : ");
        Serial.println(score);
        Serial.println("aloite peli uudelleen");
      while(digitalRead(6)==1){

      }
      for(int i =0; i<2; i++){
      setAllLeds();
      delay(300);
      clearAllLeds();
      delay(300);
      }
      delay(500);
    startTheGame();
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
  time=10;
  score=0;

}

void startTheGame()
{
   // see requirements for the function from SpedenSpelit.h
    TIMSK1 |= 0b00000010; //enable button interrupts
  interrupts();
initializeGame();
randomSeed(analogRead(A0));
}

