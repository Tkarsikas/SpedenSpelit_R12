#include "buzzer.h"

volatile int savel=0;
volatile unsigned long breakTimer=0;


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
    case 4:
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
  

void idleMelody(){

  static int indexer=0;
  static unsigned long currentMillis = millis();
  static bool noteStatus=false;

/*  int idleMelody[] = {
  523, 659, 784, 1047,   // C5, E5, G5, C6
  988, 784, 880, 698,    // B5, G5, A5, F5
  659, 523, 587, 784     // E5, C5, D5, G5
};*/  
//int idleMelody[] = {523, 659, 784, 659, 587, 698, 880, 698}; //odotus musiikki kun peli on odottaa käynnistystä

int idleMelody[] = {
  523, 587, 659, 698,  // C5, D5, E5, F5
  659, 587, 523, 587,  // E5, D5, C5, D5
  523, 659, 698, 784,  // C5, E5, F5, G5
  784, 698, 659, 587   // G5, F5, E5, D5
};


if(noteStatus==false){
  tone(A0, idleMelody[indexer]);
  noteStatus=true;
  currentMillis=millis();
}
if(noteStatus==true && millis()-currentMillis>=250){
  noTone(A0);
  indexer++;
  noteStatus=false;
    if(indexer>=sizeof(idleMelody)/sizeof(idleMelody[0])){
    indexer=0;
    }
  }
}
void idleMelody2(){
int savelienTaajuudet[26] = {
  330, 294, 262, 294, 330,
  330, 330, 294, 294, 294,
  330, 392, 392, 330, 294,
  262, 294, 330, 330, 330,
  294, 294, 330, 294, 262,
  0};

  // Yksittäisten sävelen soinnin kesto
  int savelienKestot[26] = {  
  450,150,300,300,300,
  300,300,300,300,300,
  300,300,300,450,150,
  300,300,300,300,300,
  300,300,300,300,300,
  1000};

  // Yksittäisten sävelien välit
  int savelienValit[26] = {  
  60,60,60,60,60,
  60,300,60,60,300,
  60,60,300,60,60,
  60,60,60,60,300,
  60,60,60,60,60,
  1000};

  unsigned long musiikinTahdistus = millis() - breakTimer;

  int tauko = 1000; // Millisekunteina tauko ennen kuin kappale alkaa soimaan uudestaan
  int savelienValienSummaus[26];
  int taulukonKoko = sizeof(savelienKestot) / sizeof(savelienKestot[0]);
  int summa = 0;
  for (int i = 0; i < taulukonKoko;i++) {
    summa = summa + savelienValit[i] + savelienKestot[i];
    savelienValienSummaus[i] = summa;
  }


  if (musiikinTahdistus > 0 && musiikinTahdistus < savelienValienSummaus[0] && savel == 0) {
      tone(A0, savelienTaajuudet[savel], savelienKestot[savel]);
      savel++;
  } 
  else if (musiikinTahdistus > savelienValienSummaus[savel - 1] && musiikinTahdistus < savelienValienSummaus[savel] && savel > 0 && savel < 25) {
      tone(A0, savelienTaajuudet[savel], savelienKestot[savel]);
      savel++;
  } 
  else if (musiikinTahdistus > (savelienValienSummaus[savel - 1] + tauko) && musiikinTahdistus < (savelienValienSummaus[savel] + tauko) && savel == 25) {
      savel = 0;
      breakTimer = millis();
  }

}


