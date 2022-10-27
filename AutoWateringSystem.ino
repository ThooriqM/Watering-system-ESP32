#include <LiquidCrystal_I2C.h>

const int lcdColumns = 16;
const int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

#define pinTemp 35
#define pinRelay 13

const int tempLowValue = 0;
const int tempHighValue = 750;
int temperatureAnalogValue = 0;
int tempC = 0;
 
const int AirValue = 3320;   //Kondisi kelembapan minimal
const int WaterValue = 1330;  //Kondisi kelembapan maksimal
const int pinMoist = 34;
int soilMoistureValue = 0;
int soilMoisturePercent = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize the LCD
  lcd.begin();
  
  // turn on LCD backlight                      
  lcd.backlight();
  pinMode(pinRelay, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  temperatureAnalogValue = analogRead(pinTemp);
  tempC = map(temperatureAnalogValue, tempLowValue, tempHighValue, 0, 100);
  tempC = tempC + 2;

  Serial.print("analog temp :");
  Serial.println(temperatureAnalogValue);
  Serial.print("Temperature : ");
  Serial.println(tempC);

  soilMoistureValue = analogRead(pinMoist); 
  soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  
  Serial.print("Moist : ");
  Serial.println(soilMoistureValue);
  
  if(soilMoisturePercent > 100){
    Serial.println("100 %");
    lcd.setCursor(0, 1);
    lcd.print("Moist : 100%");
  }else if(soilMoisturePercent < 0){
    Serial.println("0 %");
    lcd.setCursor(0, 1);
    lcd.print("Moist : 0%");
  }else if(soilMoisturePercent >= 0 && soilMoisturePercent <= 100){
    Serial.print(soilMoisturePercent);
    Serial.println("%");
    lcd.setCursor(0, 1);
    lcd.print("Moist : ");
    lcd.print(soilMoisturePercent);
    lcd.print("%");
  } 

  lcd.setCursor(0, 0);
  lcd.print("Temp : ");
  lcd.print(tempC);
  lcd.print((char)223);
  lcd.print("C");


  if(soilMoisturePercent <= 20) {
    if (tempC <= 32) {
      digitalWrite(pinRelay, LOW);
      Serial.println("nyala");
      delay(5000);
      digitalWrite(pinRelay, HIGH);
      Serial.print("mati gez");
      delay(5000);
    }else{
      digitalWrite(pinRelay, HIGH);
    }
  }else{
    digitalWrite(pinRelay, HIGH);
  }
  
  delay(3000);
}
