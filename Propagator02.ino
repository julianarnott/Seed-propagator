/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Description:
Pot sets required temperature on lcd display.
Temperature probe checks temperature. 
Relay switches heater off when temperature is reached. 
There is a three second delay before the process repeats.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pins:
Aref  connect to 5v
A0    temperature set pot wiper - the other pins (1&3) go to Gnd & 5v respectively 
D11   Relay activate pin
D3    Tied high via 4k7 (probe yellow wire) 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 3 //(Connect to Pin 3 )
OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int temppot = 0;                // temp value pot pin
int relay = 11;                 // relay pin
int temperature_set = 0;        // value from 20 to 35 degrees C
float temperature = 0.0;        // Temperature output variable
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);

  lcd.init(); 
  lcd.init();
  lcd.backlight();
  lcd.clear(); //Clears the LCD screen and positions the cursor in the upper-left corner
  lcd.setCursor(0,0); lcd.print("EcoPropogator");
  lcd.setCursor(0,1); lcd.print("initializing"); 
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Temp set:");
  lcd.setCursor(0,1); lcd.print("Act temp:");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  SetTemperature();                                       // Set the required temperature via the pot
   lcd.setCursor(9,0); lcd.print("   ");
  lcd.setCursor(9,0); lcd.print(String(temperature_set) + char(223) + "C");
  ReadTemperature();   
  // Read temperature probe
  lcd.setCursor(9,1); lcd.print("  ");
  lcd.setCursor(9,1); lcd.print(String(temperature) + char(223) + "C");
  if (temperature <= temperature_set){                    // Switch the relay on while the temperature probe reads less than the set temperature
    digitalWrite(relay, LOW);                            // Switch the heater relay on
    lcd.setCursor(15,0); lcd.print("*");
  }else{
    digitalWrite(relay, HIGH);                             // The temperature is above the set value so switch the heater relay off
    lcd.setCursor(15,0); lcd.print(" ");
  } 
  delay(3000);                                            // Wait three second before reading again	                                 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetTemperature(){
// sets the temperature via the pot  
  int mapTemp;
  temperature_set = analogRead(temppot);
  mapTemp = map(temperature_set, 0, 1023, 18, 38);      // 28 degrees C is about the right temperature for seedlings
  temperature_set = mapTemp;
  //Serial.print("Temperature_set: "); Serial.println(temperature_set);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ReadTemperature(){
// Read the temperature from the MAX6675 probe
  sensors.requestTemperatures(); // Send the command to get temperatures
  temperature = sensors.getTempCByIndex(0);
  //Serial.print("Current Temperature: "); Serial.println( temperature );
}
