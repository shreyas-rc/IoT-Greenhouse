#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "DHT.h"

#define M_Sensor A5
#define DHTPIN 8
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial mySerial(13, 10);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int FAN = 10;
int relay = 9;
int bulb = 13;

void setup() 
{
  
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();

  pinMode(bulb,OUTPUT);
  pinMode(relay,OUTPUT);
  pinMode(FAN,OUTPUT);
  pinMode(12,OUTPUT);

}

void loop() 
  {

  //DHT Sensor(Temperature and Humidity)
  //Followed by if statements to turn fan on and off
  //Also Heater on/off
  //Based on Temperature readings
  lcd.setCursor(0, 0);
  lcd.print("Temp:    Hum:");
  lcd.setCursor(0, 1);
  int h = dht.readHumidity();
  int f = dht.readTemperature();
    
  if(f>28){
  digitalWrite(FAN,HIGH);
  lcd.setCursor(0, 1);
  lcd.print("Fan is on");
  }
  else{
    digitalWrite(FAN,LOW);
  lcd.setCursor(0, 1);
  lcd.print("Fan is off");
  }
  lcd.setCursor(6,0);
  lcd.print(f);
  lcd.setCursor(14,0);
  lcd.print(h);  
  delay(1000);
  lcd.clear();

  if(f<15){
    digitalWrite(bulb,HIGH);
    lcd.setCursor(0, 1);
  lcd.print("Heater is on");
  delay(1000);
  }
  lcd.clear();
  
  //LDR Sensor Block and related if statements
  int ldr=analogRead(A0);
  lcd.print("Lux is");
  lcd.setCursor(7,0);
  lcd.print(ldr);
  delay(1500);
  lcd.clear();
  if(ldr>340 && ldr<900)
  {
  digitalWrite(relay,HIGH);
  lcd.setCursor(0,0);
  lcd.print("Bulb on");delay(1000);
  }
  else {digitalWrite(relay,LOW);lcd.print("Bulb off");delay(1000);}
  lcd.clear();
  
  //PIR Sensor block and GSM Module integration
  //GSM Module sends message only on intruder detection
  //No other block needs GSM as the functions are monitored automatically 
  int pirstate=digitalRead(11);
  lcd.setCursor(0,0);
  if(pirstate==HIGH)
    {
    lcd.print("Intruder Alert!");
    delay(1000);
    lcd.clear();
    lcd.print("Sending Alert");
    lcd.setCursor(0,1);
    lcd.print("Status to mob");
    delay(1000);
    Serial.println();
    Serial.println("AT+CMGF=1");
    delay(300);
    Serial.println();
    Serial.print("AT+CMGS=\"+919739684409\""); 
    Serial.println();
    delay(300);
    Serial.print("Intruder Found");
    delay(100);
    Serial.println();
    Serial.write(26);
    lcd.clear();
    lcd.print("Status updated");
    lcd.setCursor(0,1);
    lcd.print("on Phone");
    delay(1000);
    lcd.clear();
    } 
  
  //Moisture Sensor Block
  float Moisture = analogRead(M_Sensor);
  lcd.print("Moist. level is");
  lcd.setCursor(0,1);
  lcd.print(Moisture);
  delay(500);
  lcd.clear(); 
  if (Moisture> 700)   // for dry soil
  { 
        lcd.setCursor(0,0);
        lcd.print("DRY");
        lcd.setCursor(5,0);
        lcd.print("SOIL");
        delay(500);
         digitalWrite(12, HIGH);
         lcd.setCursor(0,1);
         lcd.print("PUMP:ON");
       }
       else
       {
         digitalWrite(12, LOW);
         lcd.setCursor(0,1);
         lcd.print("PUMP:OFF");
       }
   if (Moisture>= 300 && Moisture<=700) //for Moist Soil
   { 
      lcd.setCursor(0,0);
     lcd.print("MOIST");
     lcd.setCursor(6,0);
     lcd.print("SOIL");
     digitalWrite(12,LOW);
     lcd.setCursor(0,1);
     lcd.print("PUMP:OFF");    
  }
 
  if(Moisture < 300)  // For Soggy soil
  { 
    lcd.setCursor(0,0);
     lcd.print("SOGGY");
     lcd.setCursor(7,0);
     lcd.print("SOIL");
     digitalWrite(12,LOW);
     lcd.setCursor(0,1);
     lcd.print("PUMP:OFF");
  }
    Serial.println("AT+CMGF=1");
    delay(300);
    Serial.println();
    Serial.print("AT+CMGS=\"+919739684409\""); 
    Serial.println();
    delay(300);
    Serial.print(f);
    delay(100);
    Serial.print(h);
    delay(100);
    Serial.print(Moisture);
    delay(100);
    Serial.print(ldr);
    delay(100);
    Serial.println();
    Serial.write(26); 
 delay(5000);
}
