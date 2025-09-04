#include "display.h"

// Numerot lähdöille Arduinon pinninumeroinnin mukaan, Globaaleja muuttujia
int serialInput = 8;
int outEnable = 9;
int latchClock = 10;
int shiftClock = 11;
int displayReset = 12;
/*
const byte numerotBitteina[10][8] = { // Segmenttien testaus, Globaali taulukko
  {1,0,0,0,0,0,0,0}, // tyhjä (Lähtö Q7, ei ole kytkettynä. Voitaisi kytkeä pisteeseen näytön alareunassa)
  {0,1,0,0,0,0,0,0}, // Keskiviiva
  {0,0,1,0,0,0,0,0}, // Vasen ylempi
  {0,0,0,1,0,0,0,0}, // Vasen alempi
  {0,0,0,0,1,0,0,0}, // Alin viiva
  {0,0,0,0,0,1,0,0}, // Oikea alhaalla
  {0,0,0,0,0,0,1,0}, // Oikea ylhäällä
  {0,0,0,0,0,0,0,1}, // Yläviiva
  Syttyvä segmentti riippuu kytkennästä. Nykyisellä kytkennällä meni näin
};
*/
const byte numerotBitteina[10][8] = { // Taulukko segmenttinäyttöojen ohjaukseen, Globaali taulukko
  {0,0,1,1,1,1,1,1}, // 0
  {0,0,0,0,0,1,1,0}, // 1
  {0,1,0,1,1,0,1,1}, // 2
  {0,1,0,0,1,1,1,1}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {0,1,1,0,1,1,0,1}, // 5
  {0,1,1,1,1,1,0,1}, // 6
  {0,0,0,0,0,1,1,1}, // 7
  {0,1,1,1,1,1,1,1}, // 8
  {0,1,1,0,1,1,1,1}, // 9
};

void initializeDisplay(void) { // See requirements for this function from display.h
// Alustetaan pinnit oikein
pinMode(latchClock, OUTPUT);
pinMode(shiftClock, OUTPUT);
pinMode(serialInput, OUTPUT);
pinMode(outEnable, OUTPUT);
pinMode(displayReset, OUTPUT);

digitalWrite(displayReset, LOW); // Resetoidaan näytöt
digitalWrite(displayReset, HIGH);
digitalWrite(outEnable, LOW); // Outputit aktiiviseksi


}

void writeByte(uint8_t bits,bool last) {   // See requirements for this function from display.h
  int taulukonNumero = 0;
  for (int i = 0; i < 8 ; i++) {
    int low_High = numerotBitteina[bits][i];
    /*
    Serial.print(("Taulukon numero on "));
    Serial.print(low_High);
    Serial.print(" numeron ");
    Serial.print(bits);
    Serial.print(" kohdalta. Menossa taulukon indeksi ");
    Serial.println(taulukonNumero);
    delay(300);
    */
    if (low_High == 0) {
      digitalWrite(serialInput, LOW);
    } else {
      digitalWrite(serialInput, HIGH);
    }
    digitalWrite(shiftClock, HIGH); // Siirrettään bitti/yhden segmentin ohjaus kontrollerille
    digitalWrite(shiftClock, LOW);
    taulukonNumero++;
  }
  if (last == true) {
    digitalWrite(latchClock, HIGH); // "Aktivoidaan" uusi syöte kunnes molemmat ykköset ja kympit on syötetty
  }
}


void writeHighAndLowNumber(uint8_t tens,uint8_t ones) { // See requirements for this function from display.h
  digitalWrite(latchClock, LOW); // "Vedetään" ensin "latchClock" alas, jotta syötetyt bittisarjat ei mene suoraan segmentti näytöille
  writeByte(tens,false); // Ensin syötetään kympit, totuusarvo "false" -> Numerot ei vielä päivity
  writeByte(ones,true); // Perään syötetään ykköset, totuusarvo "true" -> Numerot päivittyy
}

void showResult(byte number) { // See requirements for this function from display.h
  uint8_t ykkoset = number % 10;  // Lasketaan tulevan arvon jakojäännös kymmenellä
  uint8_t kympit = number / 10;  // Jaetaan tuleva arvo kymmenellä -> saadaan kymmenet
  writeHighAndLowNumber(kympit,ykkoset);
}

