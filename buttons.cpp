#include "buttons.h"




void initButtonsAndButtonInterrupts(void)
{
  // See requirements for this function from buttons.h
  int button[] = {2,3,4,5,6};
  for(int i =0; i<5; i++){
    pinMode(button[i], INPUT_PULLUP); //pinnien ylösveto
  }
  PCICR |= 0b00000100; //enable pcmsk2 eli pinnit 0-7 voi tehdä keskeytyksen
  PCMSK2 |= 0b01111100; //seuraa pinnejä 2-6
}

ISR(PCINT2_vect) {
   /*
     Here you implement logic for handling
	 interrupts from 2,3,4,5 pins for Game push buttons
	 and for pin 6 for start Game push button.
   */
   static byte portDMSK = 0b01111100; //0b01111100 seuraa D-portin pinnien statusta jatkuvasti
  
   /*Kun joku nappi painetaan niin PIND tavussa muuttuu nappia vastaava bitti.
   xor-operaatiolla verrataan lähtötilannetta napin painallus hetkeen. jos muutosta ei tapahdu xor operaation tulos = b00000000 */
   byte currentState = PIND;
   byte changedBit = portDMSK ^ currentState; 
   int nappi=-1;
   if(changedBit){
    for(int i =2; i<=6; i++){
      if(bitRead(changedBit, i) && (!bitRead(currentState, i))){
            Serial.print("nappi ");
            nappi +=i;
            Serial.print(nappi);
            Serial.println(". painettu");
      }
    }
   }
   
   
   /*if(changedBit & 0b00000100 ){ // jos 1. nappi painettu changedBit=0b00000100 -> 0b00000100 and 0b00000100 = 1 eli true
     if(!(PIND & 0b00000100 )){
      //delay(30);
       // Serial.print(changedBit);
        Serial.println("1. nappi painettu");
        delay(30);
      }
    }
    if(changedBit & 0b00001000 ){ // jos 2. nappi painettu changedBit=0b00001000 -> 0b00001000 and 0b00001000 = 1 eli true
     if(!(PIND & 0b00001000 )){
      delay(30);
             Serial.println("2. nappi painettu");

      }
    }
    if(changedBit & 0b00010000 ){ // jos 3. nappi painettu changedBit=0b00010000 -> 0b00010000 and 0b00010000 = 1 eli true
     if(!(PIND & 0b00010000 )){
      delay(30);
      Serial.println("3. nappi painettu");

      }
    }
    if(changedBit & 0b00100000 ){ // jos 4. nappi painettu changedBit=0b00100000 -> 0b00100000 and 0b00100000 = 1 eli true
     if(!(PIND & 0b00100000 )){
      delay(30);
           Serial.println("4. nappi painettu");

      }
    }
    if(changedBit & 0b01000000 ){ // jos 5. nappi painettu changedBit=0b01000000 -> 0b01000000 and 0b01000000 = 1 eli true
     if(!(PIND & 0b01000000 )){
      delay(30);
     Serial.println("5. nappi painettu");

      }
    }*/
    
}