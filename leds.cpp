#include "leds.h"
#include "buzzer.h"

/*
Tämä moduuli aktivoi pinneihin A2-A5 kytketyt ledit ja ledien käsittelyyn on määritelty funktioita
-setLed sytytä yksi ledi
-clearAllLeds sammuta kaikki ledit
-setAllLeds sytytä kaikki ledit
-show1 ja show2 joissa välkytellään ledejä
*/
void initializeLeds() //alustaa ledit
{
pinMode(A2, OUTPUT);      //määritellään pinnit A2-A5 output
pinMode(A3, OUTPUT);
pinMode(A4, OUTPUT);
pinMode(A5, OUTPUT);

}

void setLed(byte ledNumber) //sytyttää yhden ledin
{
int analogPin[] = {A2,A3,A4,A5};

clearAllLeds();
delay(30);                                    //odotetaan hetki jotta pelaaja huomaa ledin vaihtumisen
if (ledNumber >=0 && ledNumber<=3){           //tarkastetaan mikä ledi annettiin parametrinä  
  digitalWrite(analogPin[ledNumber], HIGH);   //sytytetään parametrinä annettu ledi
  playTone(ledNumber);
  }
}


void clearAllLeds()//sammuttaa kaikki ledit
{
 int analogPin[] = {A2,A3,A4,A5};
 for(int i = 0; i<4; i++){                  //sammutetaan kaikki ledit
  digitalWrite(analogPin[i], LOW);
  }
}

void setAllLeds() //sytyttää kaikki ledit
{
int analogPin[] = {A2,A3,A4,A5};
for(int i = 0; i<4; i++){                   //sytytetän kaikki ledit
  digitalWrite(analogPin[i], HIGH);
  }
}


void show1() //välkyttelee ledejä 0-15 bitteinä
{
int analogPin[] = {A2,A3,A4,A5};

for (int value =0; value < 16; value++){                  //käydään kaikki luvut läpi jotka halutaan näyttää bitteinä
  
  for(int led =0; led<4; led++){                          //käydään kaikki ledit läpi 
    digitalWrite(analogPin[led], (value >> led) & 0x01);  //bitin siirrolla asetetaan lukua vastaavasti bitit LOW tai HIGH
    //delay(50);                                          //esim value 10 >> led 0    0000 1010 siirretään bittejä 0 kertaa oikealle -> (0000 1010) AND 0000 0001 joten 0 led -> LOW  
    if((value >> led) & 0x01){                            //esim value 10 >> led 1    0000 1010 siirretään bittejä 1 kertaa oikealle -> (0000 0101) AND 0000 0001 joten 1 led -> HIGH 
      //playTone(led);                                      //esim value 10 >> led 2    0000 1010 siirretään bittejä 2 kertaa oikealle -> (0000 0010) AND 0000 0001 joten 2 led -> LOW              
    }                                                     //esim value 10 >> led 3    0000 1010 siirretään bittejä 3 kertaa oikealle -> (0000 0001) AND 0000 0001 joten 3 led -> HIGH
  }                                                       //palataan ensimmäiseen for looppiin ja käsitellään seuraava luku samalla tavalla
  delay(500);                                              
}                                                         
}                                                         

void show2(int rounds) //välkyttelee ledejä nopeutuvasti
{                                                         //käyttäjä syöttää luvun, kuinka monta sykliä haluaa ledejä välkytettävän
int analogPin[] = {A2,A3,A4,A5};
int minDelay =0;                                          //määritellään minimi delay
int maxDelay =200;                                        //määritellään maximi delay
int delayDecrease = (maxDelay/rounds)+50;                 //lasketaan parametrinä annettuun lukuun suhteutettuna kuinka monta sykliä ledejä välkytellään

while(maxDelay>minDelay){                                 //niin kauan kun maximi delay on isompi kun minimi delay
 for(int led=0;led<4;led++){
  digitalWrite(analogPin[led], HIGH);                     //sytytellään ja sammutellaan ledejä  
  playTone(led);                                          //soitetaan lediä vastaava ääni                  
  delay(maxDelay); 
    digitalWrite(analogPin[led], LOW);
    delay(maxDelay);

}
    maxDelay -=delayDecrease;                             //vähennetään maximi delay ajasta parametrinä suhteutettu luku

 }
}

