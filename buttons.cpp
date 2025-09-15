#include "buttons.h"

byte pinnienAlkuArvot[5];

void initButtonsAndButtonInterrupts(void) {
  for(int i = firstPin; i <= lastPin;i++) { // Asetetaan pinnit haluttuun tilaan
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(startPin, INPUT_PULLUP); // Asetetaan pelin aloitus nappula haluttuu ntilaan

  PCICR = 0b00000100; // vain PCIE2 päälle
  PCMSK2 = 0b01111100; // PCINT18–22 (pinnit 2-6) päälle, muut nollaksi

  for (int i = 2; i <= 6;i++) { // Luetaan pinnien arvot kun napit ylhäällä
    pinnienAlkuArvot[i-2] = digitalRead(i);
  }
}

ISR(PCINT2_vect) {

  
/*
ISR(PCINT2_vect) {
   /*
     Here you implement logic for handling
	 interrupts from 2,3,4,5 pins for Game push buttons
	 and for pin 6 for start Game push button.
   */

  unsigned long now = millis();

  // Haetaan nappia painaessa, mitä nappia on painettu ja resetoidaan timer
  if (now - interruptTimer > 20 && buttonNumber == -1) {
    int kierrokset = 0;
    while(buttonNumber == -1) {
      for (int i = firstPin ; i <= startPin;i++) {
        if (digitalRead(i) == 0) {
          buttonNumber = i - 2;
          interruptTimer = now;
          break;
          }
        }
      if (millis() - now > 200 || kierrokset > 500) {
        Serial.println("Jäin jumiin napin lukuun. Edellisen painalluksen luku epäonnistui");
        Serial.println("Paina edelelistä nappia uudelleen");
        buttonNumber = -1;
        break;
      }
      kierrokset++;
    }
  }

  // Napin noustessa, palautetaan buttonNumber = -1 ja resetoidaan timer
  if (now - interruptTimer > 10 && buttonNumber == -2) {
    // Serial.println("Nappi noussut ylös");
    buttonNumber = -1;
    interruptTimer = now;
  }
}

