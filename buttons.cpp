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

  if(now - interruptTimer > 10 && buttonPressed == false){    // aikavertailu (debounce) kun nappi painetaan low tilaan, muita ehtoja lisänä
      for(int i=firstPin; i<=startPin;i++){
          while(digitalRead(i)==0){                  //luetaan kaikki pinnit ja jos joku pinneistä 0 niin edetään
              buttonPressed = true;                  //asetetaan napin painallus tila true
              buttonNumber = i - 2;                 //tallennetaan napin numero muuttujaan
              interruptTimer = now;                //tallennetaan painallus hetken aika muuttujaan
              break;                                 //keskeytetään jos joku nappi on low tilassa. Mahdollistaa vain yhden napin painalluksen
        }
      }
    }

    if(now - interruptTimer > 10 && buttonPressed == true){                   //kun nappi nostetaan tehdään myös debounce
      interruptTimer = now;                        //Tallennetaan painallus hetken aika muuttujaan
      buttonPressed=false;                           //asetetaan napin painallus tila takaisin false jotta seuraava painallus on mahdollista rekisteröidä
      // buttonNumber = -1;                              //nappi numero takaisin -1 jotta seuraava painalus on mahdollista rekisteröidä 
    }
  }


/*
  // Haetaan nappia painaessa, mitä nappia on painettu ja resetoidaan timer
  if (now - interruptTimer > 20 && buttonNumber == -1) {
    int kierrokset = 0;
    while(buttonNumber == -1) {
      kierrokset++;
      for (int i = firstPin ; i <= startPin;i++) {
        if (pinnienAlkuArvot[i-2] != digitalRead(i)) {
          // Serial.println("Kävin lukemassa pinnit");
          // newTimerInterrupt = true;
          buttonNumber = i - 2;
          interruptTimer = now;
          break;
          }
        }
      if (millis() - now > 200 || kierrokset > 100) {
        Serial.println("Jäin jumiin napin lukuun. Edellisen painalluksen luku epäonnistui");
        Serial.println("Paina edelelistä nappia uudelleen");
        buttonNumber = -1;
        break;
      }
    }
  }

  // Napin noustessa, palautetaan buttonNumber = -1 ja resetoidaan timer
  if (now - interruptTimer > 10 && buttonNumber == -2) {
    // Serial.println("Nappi noussut ylös");
    buttonNumber = -1;
    interruptTimer = now;
  }
}
*/
