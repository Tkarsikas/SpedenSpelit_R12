#include "display.h"

// Numerot lähdöille Arduinon pinninumeroinnin mukaan, Globaaleja muuttujia
int serialInput = 8;
int outEnable = 9;
int latchClock = 10;
int shiftClock = 11;
int displayReset = 12;

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
  /*
  Tehdään ensin kahdeksan numeron taulukko
  Selvitetään onko numero alle vai yli 10
  Luetaan luotuun taulukkoon segmenttinäytöille ohjaus
  Riippuen oliko arvo alle vai yli 10, muutetaan indeksin arvo 0 ykköseksi
  -> Sytytetään myös piste palamaan. Visioi pisteitä jos pisteet yli 100 tai 200
  */
  byte numero[8];
  if (bits < 10) {
    for (int i = 0; i < 8 ; i++) {
      numero[i] = numerotBitteina[bits][i];
      }
    } else {
      bits -= 10;
      for (int i = 0; i < 8 ; i++) {
        numero[i] = numerotBitteina[bits][i];
      numero[0] = 1;
    }
  }

  /*
  Lähetetään numero segmenttinäytöille
  Käydään läpi jokainen binääri millä ohjataan segmenttinäyttöjä
  ja lähetetään ne yksitellen serial-to-parallel muuntimelle,
  joka puolestaan ohjaa segmenttinäyttöjä
  */
  for (int i = 0; i < 8 ; i++) {
    int low_High = numero[i];  
    if (low_High == 0) {
      digitalWrite(serialInput, LOW);
    } else {
      digitalWrite(serialInput, HIGH);
    }
    digitalWrite(shiftClock, HIGH); // Siirrettään bitti/yhden segmentin ohjaus kontrollerille
    digitalWrite(shiftClock, LOW);
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
  uint8_t ones = number % 10;  // Lasketaan tulevan arvon jakojäännös kymmenellä
  uint8_t tens = number / 10;  // Jaetaan tuleva arvo kymmenellä -> saadaan kymmenet

  /* 
  Jos pisteiden jakojäännös yli 20 -> pisteitä yli 200
  Tällöin muutetaan ykkösten arvoa +10, ja vähennetään kymmenien jakojäännöksestä 10
  Näin sadaan kympit ja ykköset välille 10-19, jolloin syttyy myös segmenttinäyttöjen piste palamaan.
  Pisteillä indikoidaan, onko pisteet välillä 100-199 / 200-299
  Jos yksi piste palaa -> 100-199. Jos kaksi pistettä -> 200-299
  */
  
  if (tens >= 20) {
    ones += 10;
    tens -= 10;
  }
  writeHighAndLowNumber(tens,ones);
}
