#include "buzzer.h"

volatile int savel=0;
volatile unsigned long breakTimer1=0;


void initBuzzer(){                                        //alustetaan summerin ohjaus pinni output
  pinMode(A0, OUTPUT);
}

void playTone (byte ledNumber){                           //parametrinä ledin numero jota vastaava ääni toistetaan.

int fanfare[] = {
  659,  
  659, 880  
};

int durations[] = {
  200,  // lyhyt dind
  100, 1000   //didiii
};                       
  switch (ledNumber){
    case 0:
    tone(A0, 200,200);
    break;
    case 1: 
    tone(A0, 400, 200);
    break;
    case 2:
    tone(A0, 600, 200);
    break;
    case 3:
    tone(A0, 800, 200);
    break;
    case 4:                                 //case 4 soitetaan fanfaari
      for (int i = 0; i < 3; i++) {
      tone(A0, fanfare[i], durations[i]);
      delay(250);
      }
      delay(500);
    break;
    default:
    tone(A0,2000,200);
    break;
  }
}
  

void idleMelody(){           //vaihtoehtoinen idlemusiikki

  static int indexer=0;                           //käytetään soittamaan taulukossa olevia taajuuksia
  static unsigned long currentMillis = millis();  //aikavertailua varten jolla saadaan jaksotettua taajuuksien soittoa
  static bool noteStatus=false;                   //Soiko joku nuotti vai ei

/*  int idleMelody[] = {
  523, 659, 784, 1047,   // C5, E5, G5, C6        //vaihtoehtoinen idlemusiikki
  988, 784, 880, 698,    // B5, G5, A5, F5
  659, 523, 587, 784     // E5, C5, D5, G5
};*/  
//int idleMelody[] = {523, 659, 784, 659, 587, 698, 880, 698}; //odotus musiikki kun peli on odottaa käynnistystä

int idleMelody[] = {                              //idlemusiikin taajuus taulukko
  523, 587, 659, 698,  // C5, D5, E5, F5
  659, 587, 523, 587,  // E5, D5, C5, D5
  523, 659, 698, 784,  // C5, E5, F5, G5
  784, 698, 659, 587   // G5, F5, E5, D5
};


if(noteStatus==false){
  tone(A0, idleMelody[indexer]);                 //soitetaan nuotti jos notestatus = false
  noteStatus=true;                               //Asetetaan notestatus true koska nuottia soitetaan
  currentMillis=millis();                        //tallennetaan muuttujaan millis aika
}
if(noteStatus==true && millis()-currentMillis>=250){  //vertaillaan onko nuottia soitettu riittävän kauan
  noTone(A0);                                         //lopetetaan soittaminen
  indexer++;                                          //mennään seuraavaan alkioon
  noteStatus=false;                                   //asetetaan nuotin soitto tilaan false
  
    if(indexer>=sizeof(idleMelody)/sizeof(idleMelody[0])){  //jos kaikki taulukon nuotit on soitettu, 
    indexer=0;                                               //nollataan indexer jotta idlemusiikki alkaa alusta
    }
  }
}


