#include "display.h"

// Numerot lähdöille Arduinon pinninumeroinnin mukaan, Globaaleja muuttujia
int serialInput = 8;
int outEnable = 9;
int latchClock = 10;
int shiftClock = 11;
int reset = 12;

byte numerotBitteina[] = { // Taulukko binäärisenä numeroista segmenttinäytöille, Globaali taulukko
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111
};

void initializeDisplay(void) { // See requirements for this function from display.h
// Alustetaan pinnit oikein
pinMode(latchClock, OUTPUT);
pinMode(shiftClock, OUTPUT);
pinMode(serialInput, OUTPUT);
pinMode(outEnable, OUTPUT);
pinMode(reset, OUTPUT);

digitalWrite(reset, LOW); // Resetoidaan näytöt
digitalWrite(reset, HIGH);
digitalWrite(outEnable, LOW); // Outputit aktiiviseksi


}

void writeByte(uint8_t bits,bool last) {   // See requirements for this function from display.h
  digitalWrite(latchClock, LOW); // "Vedetään" ensin "latchClock" alas, jotta syötetyt bittisarjat ei mene suoraan segmentti näytöille
  shiftOut(serialInput, shiftClock, MSBFIRST, numerotBitteina[bits]); // Läheteään bitit segmenttinäytöille
  if (last == true) {
    digitalWrite(latchClock, HIGH); // "Aktivoidaan" uusi syöte kunnes molemmat ykköset ja kympit on syötetty
  }
}


void writeHighAndLowNumber(uint8_t tens,uint8_t ones) { // See requirements for this function from display.h
  writeByte(tens,false); // Ensin syötetään kympit, totuusarvo "false" -> Numerot ei vielä päivity
  writeByte(ones,true); // Perään syötetään ykköset, totuusarvo "true" -> Numerot päivittyy
}

void showResult(byte number) { // See requirements for this function from display.h
  uint8_t ykkoset = number % 10;  // Lasketaan tulevan arvon jakojäännös kymmenellä
  uint8_t kympit = number / 10;  // Jaetaan tuleva arvo kymmenellä -> saadaan kymmenet
  writeHighAndLowNumber(kympit,ykkoset);
}

