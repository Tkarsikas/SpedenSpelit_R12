#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#include "omatOhjelmat.h"
#include <EEPROM.h>

volatile float timerinNopeus = 15624;      // Aloitusarvo 1Hz taajuudelle
volatile int8_t buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false;  // for timer interrupt handler
volatile bool buttonPressed = false;
volatile unsigned long interruptTimer = 0; // Ajastin, milloin nappia painetaan
volatile byte score = 0; // Pistetilanteen tallennus
volatile bool gameStatus = false; // Seurataan, onko peli aktiivinen vai ei
byte arvotutLedit[30]; // Tähän tallennetaan aina mikä ledi on arvottu
byte painetutNapit[30]; // Tähän tallennetaan painetut napit järjestyksessä
byte arvonnanJarjestysNumero = 0; // Seurataan, monesko arvonta menossa
byte painalluksenJarjestysNumero = 0; // Seurataan, monesko napin painallus menossa
byte highScore = 0; // Seurataan ennätystä
const byte buzzerPin = 7;

volatile unsigned long breakTimer = 0;
volatile int savel = 0; // Musiikin soittamiseen aloittava nuotti

void setup() { // initialize here all modules
  Serial.begin(9600);
  initializeDisplay();
  initializeLeds();
  initButtonsAndButtonInterrupts();
  pinMode(buzzerPin, OUTPUT); // Kaijuttimen pinni
  digitalWrite(buzzerPin, LOW); // Kaijuttimen pinni nollaksi, jottei räpsähdä
  Serial.println("----- Tervetuloa SpedenSpeliin! -----");
  EEPROM.get(0, highScore);
  Serial.print("Tämän hetken ennätys on: ");
  Serial.println(highScore);
  Serial.println("Aloita peli painamalla valkoista nappia");
}

void loop() {

// --- TÄSTÄ ALKAA TOIMIVA PELI --- HUOM!! Usean rivin kommentoinnit korjattava

  if (gameStatus == false) {
    gameBreak();
  }

  if (buttonNumber >= 0) {
    // start the game if buttonNumber == 4
    // check the game if 0<=buttonNumber<4
    // Serial.print("Painettu nappi on ");
    // Serial.println(buttonNumber);
    if (buttonNumber == 4) {
      initializeGame();
    }

    if (buttonNumber < 4 && buttonNumber >= 0 && gameStatus == true) {
      painetutNapit[painalluksenJarjestysNumero] = buttonNumber;
      checkGame(buttonNumber);
      // summeri(buttonNumber);
    }
    buttonNumber = -1;
  }

  if (newTimerInterrupt == true) {
    clearAllLeds();

    // Varmistetaan, että ei tule peräkkäin samaa lediä
    byte satunnainen;    
    while (true) {
      satunnainen = random(0,4);
      if (satunnainen != arvotutLedit[arvonnanJarjestysNumero-1]) {
        break;
      }
    }
    summeri(satunnainen);
    // Serial.print("Arvottu led on: ");
    // Serial.println(satunnainen);
    setLed(satunnainen);
    arvotutLedit[arvonnanJarjestysNumero] = satunnainen;
    /*
    Serial.print("Arvottujen ledien tauluun laitettu kohtaan ");
    Serial.print(arvonnanJarjestysNumero);
    Serial.print(" numero ");
    Serial.println(satunnainen);
    */
    arvonnanJarjestysNumero++;
    if (arvonnanJarjestysNumero == 29) {
      arvonnanJarjestysNumero = 0;
    }
    newTimerInterrupt = false;

    /*
    Aina kun arvonta on jaollinen tasan kymmenellä
    Nopeutetaan kierrosta 10% */
    if (arvonnanJarjestysNumero % 10 == 0) {
      noInterrupts(); 
      timerinNopeus = timerinNopeus / 1.1;
      OCR1A = (long)timerinNopeus;
      interrupts();
    }
  } 
}

void initializeGame() {
	// see requirements for the function from SpedenSpelit.h
  // Alustetaan kaikki pelin muuttujat
  score = 0; 
  arvonnanJarjestysNumero = 0;
  painalluksenJarjestysNumero = 0;
  gameStatus = true; // Pelin status true-tilaan
  timerinNopeus = 15624; // Alusteaan ledin vilkkumis nopeus 1Hz taajuuteen
  // Nollataan taulukot
  for (int i = 0; i < 30;i++) {
    arvotutLedit[i] = -1; // Alustetaan arvotutLedit-taulukko. Kaikki "-1"-arvoon
    painetutNapit[i] = -1; // Alustetaan painetutNapit-taulukko. Kaikki "-1"-arvoon
  }
  // Vilkutetaan ledejä ennen kuin peli alkaa
  for (int i = 0; i < 4;i++) {
    if (i < 3) {
      tone(buzzerPin, 550, 400);
      setAllLeds();
      delay(400);
      clearAllLeds();
      delay(150);
    }
    if (i == 3) {
      tone(buzzerPin, 1750, 600);
      setAllLeds();
      delay(600);
      clearAllLeds();
      delay(150);
    }
  }

  initializeTimer(); // Kutsutaan aikakeskeyttimen alustamista

  Serial.println("----- GAME HAS STARTED -----");
  showResult(score);
}

void initializeTimer() {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12); // Aikakeskeytin "CTC-tilaan"

  TCNT1 = 0;
  OCR1A = timerinNopeus; // TimerinNopeus on muuttujana, nopeutetaan joka 10:n arvonnan jälkeen

  // "Pre-scaleriksi" 1024
  TIMSK1 |= (1 << OCIE1A); // Aktivoidaan aikakeskeytys
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  interrupts();
}

void checkGame(byte nbrOfButtonPush) {
  
  /* Vertailu, onko arvottu LED sekä napin painallus oikea. Jos on, 
  nostetaan pisteitä sekä painalluksen järjestysnumero sekä päivitetään pistenäyttö */
  /*
  Serial.print("Painettu nappi on: ");
  Serial.println(buttonNumber);
  Serial.print("Painalluksen järjestysnumero on: ");
  Serial.println(painalluksenJarjestysNumero);
  Serial.print("Pisteesi tällä hetkellä on: ");
  Serial.println(score);
  Serial.print("Napinpainallus taulukossa on: ");
  Serial.println(painetutNapit[painalluksenJarjestysNumero]);
  Serial.print("Arvottu LED taulukossa on: ");
  Serial.println(arvotutLedit[painalluksenJarjestysNumero]);
  */
  if (painetutNapit[painalluksenJarjestysNumero] == arvotutLedit[painalluksenJarjestysNumero]) {
    score++;
    painalluksenJarjestysNumero++;
    if (painalluksenJarjestysNumero == 29) {
      painalluksenJarjestysNumero = 0;
    }
    showResult(score);
    // Tsemppausta joka viidennen pisteen kohdalla
    if (score % 5 == 0) {  
      int zemppi = random(0,5);
      // tulostaZemppiTeksti(zemppi);
    }

  } else {
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 |= (0 << OCIE1A); // Keskeytetään aika keskeytykset
    clearAllLeds();  // Sammutetaan ledit
    gameStatus = false; // Pelin status false-tilaan -> Reagoi vain nappiin 4
    buttonNumber = -1; // Palautetaan buttonNumber = -1 arvoon
    breakTimer = millis(); // Luetaan pelin loppumisen aika, jotta taukomusiikki soi oikein sekä highscore/score näkyy oikein
    savel = 0; // Nollataan aloittava nuotti musiikin soittamista varten
    Serial.println("----- GAME OVER -----");
  
    // jos pisteet on yli vanhan ennätyksen, päivitetään ennätys
    if (score > highScore) {
      highScore = score;
      EEPROM.put(0, highScore);
      Serial.println("Teit uuden ennätyksen!"); Serial.println(""); 
    }
    // Tulostetaan lyhyt yhteenveto pelistä
    Serial.print("Pisteesi oli "); Serial.println(score);
    Serial.print("Kaikkien aikojen ennätys on "); Serial.println(highScore);
    Serial.println("Aloita peli uudestaan painamalla valkoista nappia");
    Serial.println("----- SPEDENSPELIT -----");
  }
}

ISR(TIMER1_COMPA_vect) {
  newTimerInterrupt = true; // Aikakeskeytyksessä päiviteään vain "aikalippu"
}

/*
void gameBreak(void) {
  showResult(score);
  delay(1000);
  showResult(highScore);
  delay(1000);
}
*/

void gameBreak(void) {

  unsigned long kesto = 10000;
  int ruudunPaivitus = 1500; // Ruudunpäivityksen kierron kesto. Puolet näkyy highscore, puolet score

    // Päivitetään pisteet 750 ms välein
  if ((millis() - breakTimer) % ruudunPaivitus < ruudunPaivitus / 2) {
    showResult(score);
  }
  if ((millis() - breakTimer) % ruudunPaivitus > ruudunPaivitus / 2) {
    showResult(highScore);
  }

  // Soitetun kappaleen taajuudet
int savelienTaajuudet[26] = {
  330, 294, 262, 294, 330,
  330, 330, 294, 294, 294,
  330, 392, 392, 330, 294,
  262, 294, 330, 330, 330,
  294, 294, 330, 294, 262,
  0};

  // Yksittäisten sävelen soinnin kesto
  int savelienKestot[26] = {  
  450,150,300,300,300,
  300,300,300,300,300,
  300,300,300,450,150,
  300,300,300,300,300,
  300,300,300,300,300,
  1000};

  // Yksittäisten sävelien välit
  int savelienValit[26] = {  
  60,60,60,60,60,
  60,300,60,60,300,
  60,60,300,60,60,
  60,60,60,60,300,
  60,60,60,60,60,
  1000};

  unsigned long musiikinTahdistus = millis() - breakTimer;

  int tauko = 1000; // Millisekunteina tauko ennen kuin kappale alkaa soimaan uudestaan
  int savelienValienSummaus[26];
  int taulukonKoko = sizeof(savelienKestot) / sizeof(savelienKestot[0]);
  int summa = 0;
  for (int i = 0; i < taulukonKoko;i++) {
    summa = summa + savelienValit[i] + savelienKestot[i];
    savelienValienSummaus[i] = summa;
  }

/*
  if (musiikinTahdistus > 0 && musiikinTahdistus < savelienValienSummaus[0] && savel == 0) {
      tone(buzzerPin, savelienTaajuudet[savel], savelienKestot[savel]);
      savel++;
  } 
  else if (musiikinTahdistus > savelienValienSummaus[savel - 1] && musiikinTahdistus < savelienValienSummaus[savel] && savel > 0 && savel < 25) {
      tone(buzzerPin, savelienTaajuudet[savel], savelienKestot[savel]);
      savel++;
  } 
  else if (musiikinTahdistus > (savelienValienSummaus[savel - 1] + tauko) && musiikinTahdistus < (savelienValienSummaus[savel] + tauko) && savel == 25) {
      savel = 0;
      breakTimer = millis();
  }
  */
}

void summeri(byte led) {
  if (led == 0) {
    tone(buzzerPin, 400,500);
  }
  if (led == 1) {
    tone(buzzerPin, 600,500);
  }
  if (led == 2) {
    tone(buzzerPin, 800,500);
  }
  if (led == 3) {
    tone(buzzerPin, 900,500);
  }
}

// --- TÄHÄN PÄÄTTYY TOIMIVA PELI -- HUOM!! Kommentoinnint korjattava

// NÄMÄ ON OMIA TESTAILUJA

 /*
  if (buttonNumber >= 0) {
    Serial.print("Painettu nappi on ");
    Serial.println(buttonNumber);
    buttonNumber = -1;
  } 
}
  /*
  for (int i = 2; i <= 6;i++) {
    digitalWrite(i, HIGH);
  }
  
  
  for (int i = 0; i < 20; i++) {
    showResult(i);
    delay(200);
  }

  int pin = firstPin;
  for(int pin = 0; pin <= 3; pin++) {
      setLed(pin);
      delay(250);
      Serial.print("Pinni päälle. Pinnin numero: ");
      Serial.println(pin);
    }
  delay(500);
  clearAllLeds();
  delay(500);
  setAllLeds();
  delay(500);
  clearAllLeds();
  delay(500);

  show1();
  
  show2(50);
  
  for (int i = 0; i < 16; i++) {
    showResult(i);
    int jaannos = i;
    if (jaannos >= 8 ) {
      jaannos = jaannos - 8;
      setLed(0);
    }
    if (jaannos >= 4) {
      jaannos = jaannos - 4;
      setLed(1);
    }
    if (jaannos >= 2) {
      jaannos = jaannos - 2;
      setLed(2);
    }
    if (jaannos >= 1) {
      setLed(3);
    }
    delay(800);
    clearAllLeds();
  }
  
  
  for (int i = 2; i <= 6;i++) {
    Serial.print("Pinnnin ");
    Serial.print(i);
    Serial.print(" arvo on ");
    Serial.println(pinnienAlkuArvot[i - 2]);
    delay(500);
  }
  */