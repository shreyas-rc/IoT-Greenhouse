#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiAvrI2c oled;

#include <SoftwareSerial.h>

SoftwareSerial gsm(3,2);

#include <DHT.h>
#define dataPin 5
#define DHTType DHT22
#include <MQ2.h>

DHT dht = DHT(dataPin, DHTType);
int pir=4;
int smoke=A0;
//int buz=13;
int ldr = A2;
int sm=A1;
//int r1=8;
//int r2=9;
int i=1000;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  
  gsm.begin(9600);
  
  pinMode(ldr , INPUT);
  pinMode(pir,INPUT);
  pinMode(smoke,INPUT);
  pinMode(sm,INPUT);
  //pinMode(buz,OUTPUT);
  //pinMode(r1,OUTPUT);
  //pinMode(r2,OUTPUT);
  //digitalWrite(buz,LOW);
  //digitalWrite(r1,HIGH);
  //digitalWrite(r2,HIGH);
  
}
void loop()
{

   oled.clear();
  
   float h = dht.readHumidity();
   float t = dht.readTemperature();
   int smoke_val=analogRead(smoke);
   int ldr_val=analogRead(ldr);
   int soil_moi_val=analogRead(sm); 
   
   oled.print("Humidity: ");
   oled.print(h);
   oled.println(" %");
   oled.println();
   
   oled.print("Temperature: ");
   oled.print(t);
   oled.println(" C");
   oled.println();
 
   oled.print("Smoke Level: ");
   oled.println(smoke_val);
   oled.println();

   delay(3000);
   oled.clear();
   
   oled.print("Soil Moisture: ");
   oled.println(soil_moi_val);
   oled.println();

   oled.print("Amount of light: ");
   oled.println(ldr_val);
   oled.println();
   
   delay(3000);

   while(i>=600000)
   {
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+918660169085\"\r"); 
    delay(1000);
    gsm.print("Temp:");
    gsm.print(t);
    gsm.println(" C");
    gsm.print("Humidity:");
    gsm.print(h);
    gsm.println(" %");
    gsm.print("Light Intensity:");
    gsm.print(ldr_val);
    gsm.println(" Units");
    gsm.print("Soil Moisture Level");
    gsm.print(soil_moi_val);
    gsm.println(" Units");
    gsm.print("Air Quality(Smoke):");
    gsm.print(smoke_val);
    gsm.println(" Units");
    gsm.write((char)26);
    delay(5000);
    i=1000;
    i=i+5000;
   }

   int pir_val=digitalRead(pir);
   if(pir_val==1)
   {
    gsm.println("ATD+918660169085;");
    delay(1000);
    gsm.write((char)26);
    delay(1000);
    oled.println("Intruder found");
    oled.println();
    oled.print("Calling");
    delay(1000);
    oled.print(".");
    delay(1000);
    oled.print(".");
    delay(1000);
    oled.println(".");
    delay(1000);
    oled.println();
    oled.println("Phone:8660169085");
    
    delay(3000);
    oled.clear();
   }
   
   i=i+10000;
   Serial.flush();
}
