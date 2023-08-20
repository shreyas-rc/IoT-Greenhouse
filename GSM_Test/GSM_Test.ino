#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);

String rtc;

void setup()

{

  mySerial.begin(9600);   // Setting the baud rate of GSM Module 

  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)

  mySerial.println("AT+CPMS=?");
  delay(100);

}

void loop()

{

  if (Serial.available()>0)
 
   switch(Serial.read())

  {

    case 's':

      SendMessage();

      break;

    case 'd':

      DialCall();

      break;

  }

 if (mySerial.available()>0)

   Serial.write(mySerial.read());

}

 void SendMessage()


{

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode

  delay(1000);  // Delay of 1000 milli seconds or 1 second

  mySerial.println("AT+CMGS=\"+918660169085\"\r"); // Replace x with mobile number

  delay(1000);

  mySerial.println(rtc);// The SMS text you want to send

  delay(100);

  mySerial.println((char)26);// ASCII code of CTRL+Z

  delay(1000);

}

 void RecieveMessage()

{

  mySerial.println("AT+CMGF=1");
  
  delay(1000);

  mySerial.println("AT+CMGR=1");
  delay(1000);

}

  void DialCall()

 {

  mySerial.println("AT+CMGD=1,4");
  
 }


 
