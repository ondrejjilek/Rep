#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);   // DHT 22  (AM2302)

const int LIGHT_SENSOR_PIN = A0; // Arduino pin connected to light sensor's  pin
const int LED_PIN          = 3;  // Arduino pin connected to LED's pin
const int ANALOG_THRESHOLD = 80;
const int TLACITKO = 4;
const int AUTOM = 1;
const int CIDLO = 7;
int analogValue;
int stav;
const int RUC_SV = 2;
const int RUC_NESV = 3;
int chk;
float hum;  //Stores humidity value
float temp;

void print_to_display(int radek, int sloupec, bool clear, String text){
  if(clear){
    lcd.clear();
  }
  lcd.setCursor(sloupec, radek);
  lcd.print(text);
}
void setup() {
  pinMode(LED_PIN, OUTPUT); // set arduino pin to output mode
  pinMode(TLACITKO, OUTPUT);
  pinMode(CIDLO, OUTPUT);
  lcd.init();         // initialize the lcd
  lcd.backlight();    // Turn on the LCD screen backlight
  stav=AUTOM;
  Serial.begin(9600);
  dht.begin();

}
void loop() {

  {
   //delay(2000);
   hum = dht.readHumidity();
   temp= dht.readTemperature();
   print_to_display(1,0,false,"testik");
   delay(1000);
  }

  if(stav==AUTOM){
    analogValue = analogRead(LIGHT_SENSOR_PIN); // read the input on analog pin
    if(analogValue < ANALOG_THRESHOLD){
      digitalWrite(LED_PIN, HIGH); // turn on LED
    }else{
      digitalWrite(LED_PIN, LOW);  // turn off LED
    }
    //print_to_display(0,0,true,"AUTO: NESVITI");
    if(analogValue > 150){
      print_to_display(0,0,false,"AUTO: NESVITI");
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
    else if(analogValue > 40){
      print_to_display(0,0,false,"AUTO: SVITI   ");
      digitalWrite(LED_PIN, HIGH);
      delay(100);
    }
  }

  if((digitalRead(TLACITKO)==HIGH) and (stav == AUTOM)){
    stav = RUC_SV;
    delay(200);
  }
  if((digitalRead(TLACITKO)==HIGH) and (stav == RUC_SV)){
    stav = RUC_NESV;
    delay(200);
  }
  if((digitalRead(TLACITKO)==HIGH) and (stav == RUC_NESV)){
    stav = AUTOM;
    delay(200);
  }
  if (stav==RUC_SV){
    print_to_display(0,0,true,"RUCNI SVIT: ANO");
    //print_to_display(1,0,false,"SVITI");
    digitalWrite(LED_PIN, HIGH);
    delay(100);
  }
  if (stav==RUC_NESV){
    print_to_display(0,0,true,"RUCNI SVIT: NE");
    //print_to_display(1,0,false,"NESVITI");
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}
