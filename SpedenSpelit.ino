#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "buzzer.h"
#include "SpedenSpelit.h"
#include <EEPROM.h>
#define MAX_STEPS 30


volatile int buttonNumber = -1;                                           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;                                  // for timer interrupt handler
volatile bool gameRunning = false;                                        // Pelin tilan seuraamiseen
volatile uint16_t timerValue = 0;                                         // Alustetaan OCR1A arvo alussa nollaksi 
volatile uint16_t seconds = 0;                                            // Alustetaan nollaksi
volatile byte score = 0;                                                  // Pelin pisteet                                  
volatile byte highscore = 0;                                              // Pelin huippupisteet
uint16_t currentStep = 0;                                                 // Pelin askel
uint16_t playerStep = 0;                                                  // Pelaajan askel
byte randomLeds[MAX_STEPS];                                               // Taulukko arvotuille ledeille
byte pressedButtonArray[MAX_STEPS];                                       // Taulukko painetuille napeille
byte lastLed = 255;                                                       // Muuttuja estämään saman ledin peräkkäinen arpominen
unsigned long breakTimer = 0;                                             // Käytetään pisteiden näyttämiseen pelin ollessa tauolla


void setup(){

  Serial.begin(9600);                                                     // Käynnistetään Arduino
  initializeDisplay();                                                    // Alustetaan näyttö
  initializeLeds();                                                       // Alustetaan ledit
  initButtonsAndButtonInterrupts();                                       // Alustetaan napit ja nappikeskeytykset
  initBuzzer();                                                           // Alustetaan kaiutin
  clearAllLeds();                                                         // Sammutetaan alussa ledit
  highscore = EEPROM.read(0);                                             // Haetaan highscore muistista
  Serial.println("Tervetuloa SpedenSpeleihin!");
  Serial.print("Tämän hetken ennätys:");
  Serial.println(highscore);          
  Serial.println("Aloita peli painamalla valkoista nappia!");                         
}  


void loop(){

  while(pressedButton == -1 && !gameRunning){                             // Soitetaan melodiaa kun mitään nappia ei ole painettua                
      idleMelody();                                                       // ja pelin tila ei ole aktiivinen
      gameBreak();
    }

  buttonNumber = pressedButton;                                

  if(pressedButton >=0){
    
    if(pressedButton == 4 && !gameRunning){                               // Kun painetaan käynnistysnappia ja peli ei ole käynnissä, alustetaan peli
    startTheGame();
    Serial.println("Peli alkaa!");
      
    }else if(gameRunning && pressedButton < 4 && pressedButton >= 0){     // Jos pelin tila on true ja painettu nappi on 0-3 eli ledejä vastaavat napit,
    pressedButtonArray[playerStep % MAX_STEPS] = pressedButton;
    checkGame(pressedButton);                                             // kutsutaan checkGame funktio jossa tarkistetaan oliko painallus oikea

    }
    pressedButton = -1;                                                   // Nollataan painallus ettei nappi jää "pohjaan"
    buttonNumber = -1;                                                    // Nollataan painallus ettei nappi jää "pohjaan"
  }

  
if(newTimerInterrupt && gameRunning){                                     // Kun peli on päällä ja tapahtuu ajastinkeskeytys
  
  if(playerStep == currentStep){
  clearAllLeds();                                                         // Sammutetaan kaikki ledit ennen uuden sytyttämistä
  }

  byte led;
  do {                                                     
    led = random(0,4);                                                    // Arvotaan uusi led ja 
  } while(led == lastLed);                                                // Varmistetaan ettei ohjelma arvo peräkkäin kahta samaa lediä

  lastLed = led;

  setLed(led);
  randomLeds[currentStep % MAX_STEPS] = led;                              // Tallennetaan taulukkoon
  currentStep++;                                                          // Pelin askel kasvaa
  newTimerInterrupt = false;                                              // Nollataan, jotta tämä osa suoritetaan vain kerran per keskeytys
  
}
}


void initializeTimer(void){

	noInterrupts();                                                          // Estetään keskeytykset
  TCCR1A = 0b00000000;                                                     // Nollataan rekisteri A
  TCCR1B = 0b00000000;                                                     // Nollataan rekisteri B
  TCNT1 = 0;                                                               // Nollataan laskuri
  OCR1A = timerValue;                                                      // Pelin alussa timerValue = 15624  -> 1Hz (16 MHz / 1024 (15624) = 1Hz)
  
  TCCR1B |= (1 << WGM12);                                                  // Asetetaan timer CTC-tilaan "Clear timer on compare match"
  TCCR1B |= (1 << CS12) | (1 << CS10);                                     // Asetetaan prescaler 1024 
  TIMSK1 |= (1 << OCIE1A);                                                 // Sallitaan keskeytys kun OCR1A saavutetaan
  interrupts();                                                            // Sallitaan keskeytykset
}


ISR(TIMER1_COMPA_vect){

  seconds++;
  newTimerInterrupt = true;

  if (seconds % 10 == 0){
    uint16_t newTimerValue = ((uint32_t)timerValue * 90)/100;             // Nopeutetaan peliä joka kymmenes keskeytys 10 %
    if (newTimerValue > 0){                                               // Varmistetaan ettei uusi timerValue voi olla nolla tai negatiivinen
      timerValue = newTimerValue;                                         // Sijoitetaan uusi arvo timerValue muuttujaan
      OCR1A = timerValue;                                                 // Päivitetään Timer1 uusi arvo
    }
  }
}

void checkGame(byte pressedButton) {

  if(pressedButtonArray[playerStep % MAX_STEPS] == randomLeds[playerStep % MAX_STEPS]){   // Tarkistetaan vastaako pelaajan painama nappi oikeaa lediä
    score++;                                                              // Kasvatetaan pisteitä yhdellä                         
    playerStep++;                                                         // Kasvatetaan pelaajan askelta
    showResult(score);                                                    // Näytetään pisteey näytöllä
  } else {
    Serial.println("Väärin!");                                            
    tone(A0,100,1500);                                                    // Jos painallus ei vastaa oikeaa lediä kaikki valot vilkkuvat kerran ja peli päättyy
    setAllLeds();                                                         
    delay(1000);
    clearAllLeds();
    showResult(score);                                                    // Näytetään tulos näytöllä
    gameRunning = false;                                                  // Pelin tila deaktivoituu

    if(score > highscore){                                                // Uuden ennätyksen tullessa soitetaa fanfaari ja tallenetaan tulos muistiin
      highscore = score;
      playTone(4);
      EEPROM.write(0, highscore);
      Serial.println("Uusi ennätys!");
    }

    Serial.print("Peli päättyi! Pisteet:");
    Serial.println(score);
    Serial.print("Ennätys: ");
    Serial.println(highscore);
  }
}


void initializeGame()
{
  gameRunning = true;                                                    // Asetetaan pelin tila aktiiviseksi
  playerStep = 0;                                                        // Nollataan pelaajan askel
  currentStep = 0;                                                       // Nollataan pelin askel
  score = 0;                                                             // Nollataan pisteet
  seconds = 0;                                                           // Nollataan sekunnit muuttuja jota käyteään pelin nopeutuksessa
  timerValue = 15624;                                                    // Asetetaan OCR1A alkuarvo
  showResult(score);                                                     // Lähetetään pisteet nolla ruudulle
  
  for(int i= 0; i < MAX_STEPS; i++){                                     // Nollataan taulukot
    randomLeds[i] = -1;
    pressedButtonArray[i] = -1;
  }
}

void gameBreak(void) {
  

  int ruudunPaivitus = 2000;                                              // Ruudunpäivityksen kierron kesto. Puolet näkyy highscore, puolet score
                                                                          
  if ((millis() - breakTimer) % ruudunPaivitus < ruudunPaivitus / 2) {    // Päivitetään pisteet 10000 ms välein
    showResult(score);
  }
  if ((millis() - breakTimer) % ruudunPaivitus > ruudunPaivitus / 2) {
    showResult(highscore);
  }
  if (millis() - breakTimer > ruudunPaivitus) {
    breakTimer = millis();
  }
}

void startTheGame(){
initializeGame();                                                         // Alustetaan peli
initializeTimer();                                                        // Alustetaan timer
}
