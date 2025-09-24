#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#define MAX_STEPS 30

// Use these 2 volatile variables for communicating between
// loop() function and interrupt handlers
volatile int buttonNumber = -1;                             // for buttons interrupt handler
volatile bool newTimerInterrupt = false;                    // for timer interrupt handler
volatile bool gameRunning = false;
volatile float timerValue = 0;                              // Alustetaan OCR1A arvo alussa nollaksi      "15624 kun peli alustetaan"
volatile uint16_t seconds = 0;                                 // Alustetaan nollaksi
volatile byte score = 0;                                    // Pelin pisteeet
volatile byte currentStep = 0;
byte randomLeds[MAX_STEPS];                                           // Arvotut ledit
byte pressedButtonArray[MAX_STEPS];                                        // Painetut napit




void setup()
{
  Serial.begin(9600);                                       //Käynnistetään Arduino
  initButtonsAndButtonInterrupts();                         //Alustetaan button keskeytykset
  initializeDisplay();                                      //Alustetaan näyttö
  initializeLeds();                                         //Alustetaan Ledit
                                    
}           
void loop()
{
  
  if(!gameRunning && buttonNumber == 4)
  {
    startTheGame();
    gameRunning = true;
    buttonNumber = -1;
     // start the game if buttonNumber == 4
     // check the game if 0<=buttonNumber<4
  }

  if(newTimerInterrupt) {
    newTimerInterrupt = false;

    byte led = random(0,4);
    randomLeds[currentStep] = led;
    checkGame(buttonNumber);
     // new random number must be generated
     // and corresponding led must be activated
  }
}

void initializeTimer(void)
{
	noInterrupts();                                             //Estetään keskeytykset
  TCCR1A = 0b00000000;                                        //Nollataan rekisteri A
  TCCR1B = 0b00000000;                                        //Nollataan rekisteri B
  TCNT1 = 0;                                                  //Nollataan laskuri
  OCR1A = timerValue;                                         //Pelin alussa timerValue = 15624  -> 1Hz (16 MHz / 1024 (15624) = 1Hz)
  
  TCCR1B |= (1 << WGM12);                                     //Asetetaan timer CTC-tilaan "Clear timer on compare match"
  TCCR1B |= (1 << CS12) | (1 << CS10);                        //Asetetaan prescaler 1024 
  TIMSK1 |= (1 << OCIE1A);                                    //Sallitaan keskeytys kun OCR1A saavutetaan
  interrupts();                                               //Sallitaan keskeytykset
  
  
}
ISR(TIMER1_COMPA_vect){
  seconds++;

  if (seconds % 10 == 0){
    uint16_t newTimerValue = ((uint32_t)timerValue * 90)/100; //Nopeutetaan peliä joka kymmenes keskeytys 10 %
    if (newTimerValue > 0){                                   //Varmistetaan ettei uusi timerValue voi olla nolla tai negatiivinen
      timerValue = newTimerValue;                             //Sijoitetaan uusi arvo timerValue muuttujaan
      OCR1A = timerValue;                                     //Päivitetään Timer1 uusi arvo
    }
  newTimerInterrupt = true;

  }

  /*
  Communicate to loop() that it's time to make new random number.
  Increase timer interrupt rate after 10 interrupts.
  */
  
}


void checkGame(byte pressedButton) {
  if(pressedButton == randomLeds[currentStep]){
    score++;
    currentStep++;
  }else{
    Serial.println("Väärin!");
    gameRunning = false;            
  }
	
}


void initializeGame()
{
  score = 0;
  initializeTimer();
  timerValue = 15624;

  for(int i= 0; i < MAX_STEPS; i++){
    randomLeds[i] = 0;
    pressedButtonArray[i] = 0;
  }


	/* see requirements for the function from SpedenSpelit.h
  Alustetaan peliin tarvittavat muuttujat napeille ja random luvuille(aluksi nollataan). Taulukot arvoille(?)
  */
}

void startTheGame(){
  initializeGame();
  currentStep = 0;
  gameRunning = true;
}


