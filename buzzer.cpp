#include "buzzer.h"

void initBuzzer(){                                        //alustetaan summerin ohjaus pinni output
  pinMode(A0, OUTPUT);
}

void playTone (byte ledNumber){                           //parametrinä ledin numero jota vastaava ääni toistetaan.
  
  int melody[] = {
  523, 659, 784, 1047,   // C5, E5, G5, C6
  988, 784, 880, 698,    // B5, G5, A5, F5
  659, 523, 587, 784     // E5, C5, D5, G5
};  
int idleMelody[] = {523, 659, 784, 659, 587, 698, 880, 698};
int fanfare[] = {
  659,  
  659, 880  
};

int durations[] = {
  200,  // lyhyt dind
  100, 1000   // nouseva sarja, viimeinen pitkä
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
    for(int i=0; i<8;i++){
      tone(A0,idleMelody[i],200);
      delay(250);
    }
    break;
    case 5:
      for (int i = 0; i < 3; i++) {
    tone(A0, fanfare[i], durations[i]);
    delay(durations[i] + 50); // pieni tauko nuottien välillä
    }
    break;
    default:
    tone(A0,2000,200);
    break;
  }
  }
