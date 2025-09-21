#include "buttons.h"
//#define deBounceTime 100

/* Tämä moduuli aktivoi D porttiin kytketyt 5 nappia pinneihin 2-6. Nappien painallus aiheuttaa keskeytyksen
Kun nappia painetaan niin keskeytys funktiossa muuttujaan pressedButton tallennetaan nappia vastaava numero 0-4
Tätä muuttujaa pressedButton voidaan käyttää sitten pääfunktiossa ehtona ja se voidaan tallentaa siellä toiseen muuttujaan
jotta sen käsitteleminen on helpompaa.
*/

volatile int pressedButton=-1;              //napin numeron tallentamiseen
volatile unsigned long lastPressTime = 0;   //aikavertailuun
volatile bool buttonPressed = false;        //napin painalluksen tila



void initButtonsAndButtonInterrupts(void)
{
  // See requirements for this function from buttons.h
  int button[] = {2,3,4,5,6};                 //nappien pinnit taulukkoon
  for(int i =0; i<5; i++){                    //käydään kaikki taulukon solut läpi
    pinMode(button[i], INPUT_PULLUP);         //pinnien ylösveto
  }
  PCICR |= 0b00000100;                        //enable pcmsk2 eli pinnit 0-7 voi tehdä keskeytyksen
  PCMSK2 |= 0b01111100;                       //seuraa pinnejä 2-6
   
}

ISR(PCINT2_vect) 
{
if(millis() - lastPressTime > 10 &&  
    buttonPressed==false){    // aikavertailu (debounce) kun nappi painetaan low tilaan, muita ehtoja lisänä
    for(int i=firstPin; i<=lastPin;i++){
        while(digitalRead(i)==0){                  //luetaan kaikki pinnit ja jos joku pinneistä 0 niin edetään
            buttonPressed = true;                  //asetetaan napin painallus tila true
            pressedButton = i - 2;                 //tallennetaan napin numero muuttujaan
            lastPressTime=millis();                //tallennetaan painallus hetken aika muuttujaan
            break;                                 //keskeytetään jos joku nappi on low tilassa. Mahdollistaa vain yhden napin painalluksen
       }
    }
  }

  if(millis() - lastPressTime > 10 
      && buttonPressed == true){                   //kun nappi nostetaan tehdään myös debounce
    lastPressTime=millis();                        //Tallennetaan painallus hetken aika muuttujaan
    buttonPressed=false;                           //asetetaan napin painallus tila takaisin false jotta seuraava painallus on mahdollista rekisteröidä
    pressedButton=-1;                              //nappi numero takaisin -1 jotta seuraava painalus on mahdollista rekisteröidä 
  }
}




    
