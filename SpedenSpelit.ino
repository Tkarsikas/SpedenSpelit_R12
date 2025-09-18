#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#include "buzzer.h"
#define bufferSize 20                     //Voidaan muuttaa nappien ja ledien taulukon kokoa


volatile int buttonNumber = -1;           //napin numeron tallentamiseen
int buttonBuffer[bufferSize]={0};         //nappien arvojen taulukko
int bufferIndexButton=0;                  //nappien arvojen tallentamiseen oikeaan soluun


int ledBuffer[bufferSize]={0};            //ledien arvojen taulukko
int leds[]={0,1,2,3};                     //ledi järjestysnumerot tallennettu
int bufferIndexLed=0;                     //ledi taulukkoon tallentaminen oikeaan soluun
int randomLedNumber;                      //ledin sytyttämiseen 
int compareIndex=0;                       //taulukon arvojen vertailuun
int score=0;                              //pisteiden laskentaan
int highScore=0;

volatile bool newTimerInterrupt = false;  //for timer interrupt handler
volatile long timerValue=0;               //OCR1A arvon tallentamiseen

int ss=0;

void setup()
{
()  Serial.begin(9600);                   //Käynnistetään arduino baudin nopeudella
  initializeDisplay();
  initButtonsAndButtonInterrupts();     //alustetaan nappi keskeytykset
  initializeLeds();                     //alustetaan ledi ohjaus              
  initBuzzer();
  initializeTimer();                    //alustetaan timer1 tekemään keskeytyksiä 1hz
  startTheGame();                       //alustetaan peli
}

void loop()
{
 /*for(int i=0; i<12; i++){
  tone(A0, melody[i], 200);
  delay(500);
}*/
 if(pressedButton==4){                  //jos pelaaja painaa pelin aloitus nappia kesken pelin niin se nollataan ettei tule bugeja
  pressedButton=-2;
  Serial.println("nappi nollattu");
 }
  if(pressedButton>-1 && pressedButton<4)
  {
  buttonNumber=pressedButton;           //tallennetaan painettu nappi toiseen muuttujaan
  pressedButton= -2;                    //muutetaan muuttujan arvo siten että seuraava keskeytys käy napin noston läpi, jotta seuraava painallus toimii oikein
  addToButtonBuffer(buttonNumber);      //lisätään napin arvo taulukkoon
  checkGame(compareIndex);              //tarkistetaan pelin tila menikö napin painallus oikein
  if(compareIndex==bufferSize){                 //nollataan muuttuja koska taulukossa ei ole kuin 10 arvoa
    compareIndex=0;
  }
  }

  if(newTimerInterrupt == true)
  {
     newTimerInterrupt=false;           //aikakeskeytys käsitelty
     clearAllLeds();                    //sammutetaan kaikki ledit jotta pelaaja huomaa ledin vaihtuneen
     delay(30);                         //odotellaan hetki jotta sammutus huomataan
     randomLedNumber=random(0,4);       //arvotaan uusi ledin arvo
     addToLedBuffer(randomLedNumber);   //lisätään arvottu ledin arvo taulukkoon
     setLed(leds[randomLedNumber]);     //sytytetään uusi ledi

    handleTimerSpeedUp();
  }
}
void handleTimerSpeedUp(void){
     ss++;                              //Lisätään muuttujan arvoon 1 jokaisella timer keskeytyksellä
  if(ss==10){                           //kun muuttuja on 10 
  ss=0;                                 //nollataan muuttuja

  timerValue-=timerValue*0.1;           //vähennetään timerValuesta 10%
  OCR1A=timerValue;                     //asetetaan OCR1A arvoksi 10% vähemmän jotta peli nopeutuu
}
}

void addToButtonBuffer(int a){
  buttonBuffer[bufferIndexButton]=a;    //asetetaan parametrinä annettu arvo taulukkoon
  bufferIndexButton++;                  //lisätään indexi muuttujaan 1 jotta seuraava arvo tallentuu seuraavaan alkioon

  if(bufferIndexButton==bufferSize){    //kun indexi on yhtä suuri kuin bufferin koko, nollataan indeksi arvo
    bufferIndexButton=0;                //tämän jälkeen ylikirjoitetaan aiemmin tallennettuja alkioita
  }
}

void addToLedBuffer(int a){
  ledBuffer[bufferIndexLed]=a;          //asetetaan parametrinä annettu arvo taulukkoon
  bufferIndexLed++;                     //lisätään indexi muuttujaan 1 jotta seuraava arvo tallentuu seuraavaan alkioon

  if(bufferIndexLed==bufferSize){       //kun indexi on yhtä suuri kuin bufferin koko, nollataan indeksi arvo
    bufferIndexLed=0;                   //tämän jälkeen ylikirjoitetaan aiemmin tallennettuja alkioita
  }
}

void initializeTimer(void)
{
  timerValue=15624;         //(16*10^6)/(1024*1.0)-1 Laskettu prescaler arvon mukaan niin että keskeytys 1hz taajuudella
  TCCR1A = 0;               //alustetaan timer 1 A rekisteri
  TCCR1B = 0;               //alustetaan timer 1 B rekisteri
  TCCR1B |= 0b00001101;     //prescaler 1024 (CS10 ja CS12 high) ja CTC (clear timer on compare match) (WGM12 high) moodi enabloitu 
  TCNT1=0;                  //alustetaan ajastin
  TIMSK1 |= 0b00000010;     //enable timer1 interrupts
  interrupts();             //aktivoidaan keskeytykset

}

ISR(TIMER1_COMPA_vect)
{
newTimerInterrupt=true;         //uusi timer keskeytys joka käsitellään pää loopissa
}


void checkGame(int index)
{
 
      if(ledBuffer[index] == buttonBuffer[index]){    // vertaillaan onko painettu nappi oikein
        score++;                                      //annetaan piste
        showResult(score);
       Serial.print("Oikein!!");
          compareIndex+=1;                            //lisätään muuttujan arvoon 1 jotta tarkistuksessa tarkastetaan oikeat taulukon arvot
      }if(ledBuffer[index] != buttonBuffer[index]){   //Vertaillaan jos painettu nappi meni väärin
      compareIndex+=1;                                //lisätään muuttujan arvoon 1 jotta tarkistuksessa tarkastetaan oikeat taulukon arvot                          
      TIMSK1 = 0;                                     //pysäytetään timer
      Serial.println("väärin, peli ohi");             //kerrotaan pelaajalle että meni väärin
      Serial.print("pisteet : ");
      Serial.println(score);
      Serial.println("aloite peli uudelleen");
      setAllLeds();
      tone(A0,100,1500);                              //väläytetään ledejä ja äänimerkki että peli päättyi
      delay(500); 
      clearAllLeds();                                 //sammutetaan ledit
  
      EEPROM.get(0, highScore);                       //haetaan aiemmin tallennetut ennätyspisteet

      if(highScore < score){                          //tallennetaan aiemman kierroksen pisteet ennätykseksi jos ennätys rikottiin
        playTone(5);
        EEPROM.put(0,score);
        Serial.print("onneksi olkoon olet tehnyt uuden ennätyksen, ennätys pisteet: ");
        Serial.println(score);
      }
      Serial.print("pisteesi ei riittänyt uuteen ennätykseen, aiempi ennätys: ");
      Serial.println(highScore);                    //kerrotaan pelaajalle jos ei tullut uutta ennätystä


    startTheGame();                                 //alustetaan peli uudestaan                                     

  }
}


void initializeGame()
{
  score=0;                          //nollataan pisteet
  timerValue=15624;                 //määritellään timer keskeytys 1hz taajuudelle 
  ss=0;
  for(int i=0; i<bufferSize; i++){
  buttonBuffer[i]=0;                //nollataan button bufferi
  ledBuffer[i]=0;                   //nollataan ledi bufferi
  }
  bufferIndexLed=0;                 //ledi taulukkoon tallentaminen oikeaan soluun
  bufferIndexButton=0;              //button taulukkoon tallentaminen oikeaan soluun
  compareIndex=0;                   //taulukon arvojen vertailuun
  
  newTimerInterrupt=false;          //alustetaan timer keskeytys lippu
  pressedButton=-1;                 //alustetaan napin arvo
  OCR1A = timerValue;               //timer1 vertailu arvoksi tallennetaan aluksi 15624
  clearAllLeds();

}

void startTheGame()
{

randomSeed(analogRead(A0));         //määritellään randomSeed lukemaan tyhjää analogista pinniä jotta random generointi on parempi
EEPROM.get(0, highScore);                     //haetaan aiemmin tallennetut ennätyspisteet

      while(digitalRead(6)==1){                     //odotellaan että pelaaja aloittaa pelin uudestaan painamalla 5 nappia
      playTone(4);
      showResult(highScore);
      delay(1000);
      showResult(score);
      delay(1000);
      }

      for(int i =0; i<2; i++){                      //pelin aloittamiseksi välkytellään ledejä
      setAllLeds();
      delay(300);
      clearAllLeds();
      delay(300);
      }
initializeGame();                   //alustetaan peli
  TCNT1=0;                                          //nollataan timer1 ajastimen arvo
  TIMSK1 |= 0b00000010;                             //käynnistetään timer1
  
}
