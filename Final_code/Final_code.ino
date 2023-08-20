#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
SSD1306AsciiAvrI2c oled;

#include <SoftwareSerial.h>

SoftwareSerial gsm(3,2);

#include <DHT.h>
#define dataPin 6
#define DHTType DHT22
#include <MQ2.h>

DHT dht = DHT(dataPin, DHTType);
int pir=4;
int smoke=A0;
int buz=13;
int ldr = 5;
//int sm=A1;
int r1=8;
int r2=9;
long i=0;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);

  pinMode(buz,OUTPUT);
  pinMode(r1,OUTPUT);
  pinMode(r2,OUTPUT);
  digitalWrite(buz,LOW);
  digitalWrite(r1,HIGH);
  digitalWrite(r2,HIGH);
  
  gsm.begin(9600);
  delay(1000);
  gsm.println("AT+CPIN?");
  delay(1000);
  gsm.println("AT+CREG?");
  delay(1000);
  gsm.println("AT+CGATT?");
  delay(1000);
  gsm.println("AT+CIPSHUT");
  delay(1000);
  gsm.println("AT+CIPSTATUS");
  delay(2000);
  gsm.println("AT+CIPMUX=0");
  delay(2000);
  
  pinMode(ldr , INPUT);
  pinMode(pir,INPUT);
  pinMode(smoke,INPUT);
  //pinMode(sm,INPUT);
  
}
void loop()
{
   int ldr_val=digitalRead(ldr);
   if(ldr_val==1)
    {
     oled.clear();
     oled.println("LED ON");
     digitalWrite(r2, LOW);
     delay(3000);
     oled.clear();
    }
   else digitalWrite(r2, HIGH);
    
   float h = dht.readHumidity();
   float t = dht.readTemperature();
   int smoke_val=analogRead(smoke);
  // int soil_moi_val=analogRead(sm);
   
   int pir_val=digitalRead(pir);
   if(pir_val==1)
   {
    digitalWrite(buz,HIGH);
    delay(2000);
    digitalWrite(buz,LOW);
    gsm.println("ATD+918660169085;");
    delay(1000);
    gsm.write((char)26);
    delay(1000);
    oled.clear();
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
    
    delay(30000);
    oled.clear();
   }
   else{ 
   digitalWrite(buz,LOW);
   oled.clear();
   oled.println("No Intruder found");
   delay(3000);
   }
   
   oled.clear(); 
  
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
   
   //oled.print("Soil Moisture: ");
   //oled.println(soil_moi_val);
   //oled.println();

   oled.print("Amount of light: ");
   oled.println(ldr_val);
   oled.println();
   
   delay(3000);
   
   if(i>=300)
   {
    oled.clear();
    oled.println("Uploading Data to");
    oled.println();
    oled.println("Cloud");
    gsm.println("AT+CSTT=\"www\"");
    delay(1000);
    gsm.println("AT+CIICR");
    delay(3000);
    gsm.println("AT+CIFSR");
    delay(2000);
    gsm.println("AT+CIPSPRT=0");
    delay(3000);
    gsm.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
    delay(6000);
    gsm.println("AT+CIPSEND");
    delay(4000);
    String str="GET https://api.thingspeak.com/update?api_key=VJNGUDR8V4LYNJIP&field1=" + String(t) +"&field2="+String(h) +"&field3="+String(ldr_val) +"&field4="+String(smoke_val); //+"&field5="+String(soil_moi_val);
    gsm.println(str);
    delay(4000);
    gsm.println((char)26);
    delay(5000);
    gsm.println();
    gsm.println("AT+CIPSHUT");
    delay(100);
    oled.clear();
    i=0;
   }

   /*while(soil_moi_val>650)
   {
    oled.clear();
    oled.println("PUMP IS ON");
    digitalWrite(r1, LOW);
    delay(3000);
    soil_moi_val=analogRead(sm);
    oled.clear();
   }
   if(soil_moi_val<650)
   {
   digitalWrite(r1, HIGH);
   }
   */

   while(smoke_val>400)
   {
    oled.clear();
    oled.println("Smoke Detected");
    digitalWrite(buz, HIGH);
    delay(3000);
    smoke_val=analogRead(smoke);
    oled.clear();
   }
   if(smoke_val<390) digitalWrite(buz, LOW);

   ldr_val=digitalRead(ldr);
   if(ldr_val==1)
   {
    oled.clear();
    oled.println("LED ON");
    digitalWrite(r2, LOW);
    delay(3000);
    oled.clear();
   }
   else digitalWrite(r2, HIGH);
   i=i+5;
   Serial.flush();
}
