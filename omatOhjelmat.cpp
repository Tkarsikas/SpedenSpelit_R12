#include "omatOhjelmat.h"

void tulostaZemppiTeksti(byte zemppi) {
  if (zemppi == 0) {
    Serial.println("Hienoa!");
  } else if (zemppi == 1) {
    Serial.println("Mihin asti pääset?");
  } else if (zemppi == 2) {
    Serial.println("Hyvin menee!");
  } else if (zemppi == 3) {
    Serial.println("Loistavaa!");
  } else if (zemppi == 4) {
    Serial.println("Jatka samaan malliin!");
  }
}