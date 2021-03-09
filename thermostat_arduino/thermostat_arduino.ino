#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
#define One_WIRE_BUS 6
OneWire oneWire(One_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 20, 4);
 
#define RELAY 11
#define BUTTON_MINOR 2
#define BUTTON_MAJOR 3
#define DELAI_MESURE_TEMPERATURE 5000
#define TIME_BOUTTON 250
 
int TmpConsign = 0;
 
void setup(void) {
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("C1");
    lcd.setCursor(3, 0);
    lcd.print(float(TmpConsign));

    Serial.begin(9600);

    pinMode(BUTTON_MINOR, INPUT);
    pinMode(BUTTON_MAJOR, INPUT);
    pinMode(RELAY, OUTPUT);
}
 
void loop() {
    static uint32_t derniereMesure = millis();
    static uint32_t timer_transition = millis();
    static float temperature = 0;

    if(!digitalRead(BUTTON_MINOR) || !digitalRead(BUTTON_MAJOR)){
        if ((millis() - timer_transition) >= TIME_BOUTTON) {
            TmpConsign += (!digitalRead(BUTTON_MINOR))? -1 : 1;
            
            lcd.setCursor(0, 0);
            lcd.print("C1");
            lcd.setCursor(3, 0);
            lcd.print(float(TmpConsign));

            Serial.println(TmpConsign);

            timer_transition = millis();
        }
    }else{
        if((millis() - derniereMesure) >= DELAI_MESURE_TEMPERATURE){
            sensors.requestTemperatures();
            temperature = sensors.getTempCByIndex(0);

            lcd.setCursor(0, 1);
            lcd.print("T1");
            lcd.setCursor(3, 1);
            lcd.print(temperature);

            derniereMesure = millis();
        }

        digitalWrite(RELAY, (temperature >= TmpConsign));
    }
}
