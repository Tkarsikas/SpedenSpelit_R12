#ifndef BUZZER_H
#define BUZZER_H
#include <arduino.h>

//summerin alustamiseen. Summeri tulee kytkeä analogpinniin A0
void initBuzzer(void);

/*
Tällä funktiolla voidaan soittaa ääni summerilla ääniä,
käyttäjä syöttää parametrinä ledin numeron ja sitä vastaava ääni toistetaan
Ääni moduuli on
*/
void playTone(byte);



#endif