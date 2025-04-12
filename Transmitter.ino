#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TinyGPS.h>
#include <Servo.h>
 #include<SoftwareSerial.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";


Servo myservo;  

int pos = 0; 




int state = 0;
const int pin = A1;
float gpslat, gpslon;

TinyGPS gps;
SoftwareSerial sgps(10, 11);
SoftwareSerial sgsm(2, 3);

void setup() {
  // put your setup code here, to run once:
pinMode(A0, INPUT_PULLUP);
pinMode(A1, INPUT_PULLUP);
pinMode(A2, INPUT_PULLUP);
pinMode(A3, INPUT_PULLUP);
 myservo.attach(9);

sgsm.begin(9600);
  sgps.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();   
 
Serial.begin(9600);
 myservo.write(180); 
 delay(500);  
  


   
}

void loop() {

   

  
  // put your main code here, to run repeatedly:
int m = analogRead(A0);
int n = analogRead(A1);
int o =digitalRead(A2);
int p =digitalRead(A3);
Serial.print(m);
Serial.print("    ");
Serial.print(o);
Serial.print("    ");
Serial.print(n);
Serial.print("    ");
Serial.println(p);
delay(100);

if(o==0)
{
  myservo.write(180); 
 delay(500); 
 
  
  }

  else if (p==0)

  {
     myservo.write(0); 
 delay(500); 
 
    }




sgps.listen();
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }


  

if(m>=500)
{
   const char text[] = "H";
  radio.write(&text, sizeof(text));
  delay(100);
  
  }

  else
{
   const char text[] = "A";
  radio.write(&text, sizeof(text));
  delay(100);
  
  }

  if(n>=700)

  {
    
    sgsm.listen();
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);
      /*Replace XXXXXXXXXX to 10 digit mobile number &
        ZZ to 2 digit country code*/
      sgsm.print("AT+CMGS=\"+919560718291\"\r");
      delay(1000);
      //The text of the message to be sent.
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      delay(1000);
      sgsm.write(0x1A);
      delay(4000);
 sgsm.println("ATD +919560718291;"); //replace x by your number
   delay(20000);
   //digitalWrite(5,HIGH);  // LED1 ON
   sgsm.println("ATH");
   delay(2000);
   Serial.println("calling.....");
  
    
    }

    else
    {
      Serial.print("");
      
      }

  
 


}