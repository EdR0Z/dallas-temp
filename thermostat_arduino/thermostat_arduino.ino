#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define One_WIRE_BUS 6
OneWire oneWire(One_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define RELAY 11 //relais alimentation ventilateurs baie
#define BUZZER 10 //buzzer signal sonore température critique
#define BUTTON_MINOR 2 //bouton diminution consigne de température
#define BUTTON_MAJOR 3 //bouton augmentation consigne de température
#define DELAI_MESURE_TEMPERATURE 1000 //délai entre deux mesures de température
#define TIME_BOUTTON 250 //delai entre deux incrémentations de la consigne de température pour l'augmentation ou la vitesse de celle-ci

int TmpConsign = 0;




void setup(void) {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("S1"); //index consigne de déclenchement du relais, colonne0, ligne0
  lcd.setCursor(3, 0);
  lcd.print(TmpConsign); //affichage consigne de déclenchement du relais, colonne3, ligne0

  lcd.setCursor(0, 1);
  lcd.print("C1"); //index température critique, colonne0, ligne1
  lcd.setCursor(3, 1);
  lcd.print(TmpConsign + 5); //affichage température critique, colonne3, ligne1

  Serial.begin(9600);

  pinMode(BUTTON_MINOR, INPUT);
  pinMode(BUTTON_MAJOR, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}




void loop() {
  static uint32_t derniereMesure = millis();
  static uint32_t timer_transition = millis();
  static float temperature = 0;

  if (!digitalRead(BUTTON_MINOR) || !digitalRead(BUTTON_MAJOR)) {
    if ((millis() - timer_transition) >= TIME_BOUTTON) {
      TmpConsign += (!digitalRead(BUTTON_MINOR)) ? -1 : 1;

      lcd.setCursor(0, 0);
      lcd.print("S1"); //index consigne de déclenchement du relais, colonne0, ligne0
      lcd.setCursor(3, 0);
      lcd.print(TmpConsign); //affichage consigne de déclenchement du relais, colonne3, ligne0

      lcd.setCursor(0, 1);
      lcd.print("C1"); //index température critique, colonne0, ligne1
      lcd.setCursor(3, 1);
      lcd.print(TmpConsign + 5); //affichage température critique, colonne3, ligne1

      timer_transition = millis();
    }
  } else {
    if ((millis() - derniereMesure) >= DELAI_MESURE_TEMPERATURE) {
      sensors.requestTemperatures();
      temperature = sensors.getTempCByIndex(0);

      lcd.setCursor(0, 2);
      lcd.print("T1"); //index temperature relevée, colonne0, ligne2
      lcd.setCursor(3, 2);
      lcd.print(float(temperature)); //affichage temperature relevée, colonne3, ligne2

      derniereMesure = millis();
    }

    digitalWrite(RELAY, (temperature >= TmpConsign));
    digitalWrite(BUZZER, (temperature >= TmpConsign + 5));
  }
}
