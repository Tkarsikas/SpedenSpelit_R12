#ifndef BUZZER_H
#define BUZZER_H
#include <arduino.h>

extern volatile int savel;
extern volatile unsigned long breakTimer1;

//summerin alustamiseen. Summeri tulee kytkeä analogpinniin A0
void initBuzzer(void);

/*
Tällä funktiolla voidaan soittaa ääni summerilla ääniä,
käyttäjä syöttää parametrinä esimerkiksi ledin numeron ja sitä vastaava ääni toistetaan
Funktiossa on lisäksi tone 4 ja 5 
*/
void playTone(byte);
void idleMelody(void);
void idleMelody2(void);



#endif