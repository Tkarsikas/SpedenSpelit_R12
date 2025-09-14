#include "display.h"


const int reset =12;        //määritellään eri ohjaus pinneille digitaalipinnit
const int shiftClock=11;
const int latchClock=10;
const int outEnable=9;
const int serialInput=8;

const uint8_t segmentMap[10] = { //määritellään 0-9 numerot segmentti näytölle
0b00111111,//0
0b00000110,//1
0b01011011,//2
0b01001111,//3
0b01100110,//4
0b01101101,//5
0b01111101,//6
0b00000111,//7
0b01111111,//8
0b01101111,//9
};
  
void initializeDisplay(void)
{
// See requirements for this function from display.h

pinMode(reset, OUTPUT);           //asetetaan pinnit OUTPUT pinneiksi
pinMode(shiftClock, OUTPUT);
pinMode(latchClock, OUTPUT);
pinMode(outEnable, OUTPUT);
pinMode(serialInput, OUTPUT);

digitalWrite(outEnable, LOW);    //asetetaan outEnable low jotta lukema näkyy
digitalWrite(reset, LOW);        //nollataan rekisteri
digitalWrite(reset, HIGH);       //asetetaan high jotta voi tallentaa uutta


}


void writeByte(uint8_t bits,bool last)
{
// See requirements for this function from display.h
byte segmentByte= segmentMap[bits];     //haetaan segmenttiMapista numeroa vastaava tavu


for(int i =0;i<8;i++){                  //käydään kaikki bitit läpi ja shitfClockilla siirretään ne registeriin

  digitalWrite(serialInput, (segmentByte>>(7-i)& 0x01));  //asetetaan serialInput tavun osoittamalla tavalla järjestyksessä MSB -> LSB 
  
  digitalWrite(shiftClock, HIGH);       //shiftClock pulssi
  digitalWrite(shiftClock, LOW);
}
if(last){                               //jos last==true niin aktivoidaan latchclock
  digitalWrite(latchClock, HIGH);       //latchClock pulssi
  digitalWrite(latchClock, LOW);

  }
}


void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
// See requirements for this function from display.h
writeByte(tens, false);       //ohjataan kympit writeByte funktiolle, bool=false koska segmentit näytetään vasta kun ykkösetkin on syötetty
writeByte(ones, true);        //ohjataan ykköset writeByte funktiolle, bool=true koska segmentit näytetään heti kun ykkösetkin on syötetty
}

void showResult(byte number)
{
  int tens = number /10;     //jaetaan luku 10 josta saadaan kymppien lukumäärä
  int ones = number %10;     //luvun jakojäännöksestä jää ykköset
  writeHighAndLowNumber(tens, ones);  //syötetään kympit ja ykköset writeHighAndLowNumber funktiolle;
// See requirements for this function from display.h
}

