#include <Wire.h>
#include <JY901.h>
#include <SoftwareSerial.h>

SoftwareSerial sim(10, 11);
String number = "Your Number";
int _timeout;
String _buffer;
const int buzzer = 12;
float x;
float y;
float z;
float vectorsum ;
/*
Test on Uno R3.
JY901    UnoR3
SDA <---> SDA
SCL <---> SCL
*/
void setup() 
{
    JY901.StartIIC();
    delay(7000); //delay for 7 seconds to make sure the modules get the signal
    Serial.begin(9600);
     pinMode(2, INPUT);
    _buffer.reserve(50);
    Serial.println("Sistem Started...");
    sim.begin(9600);
    delay(1000);
  } 

void loop() 
{
  JY901.GetAcc();
  int button = digitalRead(2);
  Serial.print(button);
  x = (float)JY901.stcAcc.a[0]/32768*16;
  y = (float)JY901.stcAcc.a[1]/32768*16;
  z = (float)JY901.stcAcc.a[2]/32768*16;
  vectorsum = sqrt(sq(x)+sq(y)+sq(z));
//  Serial.print(vectorsum);
//
//  Serial.print("x : "); Serial.print(x); Serial.print(", ");
//  Serial.print("y : "); Serial.print(y); Serial.print(", ");
//  Serial.print("z : "); Serial.print(z); Serial.print(", ");
  Serial.print("Acc : "); Serial.print(vectorsum); Serial.print(", ");
  if((vectorsum > 5.0) || (button == 1)){
    Serial.print("Check");
    buzz();
    SendMessage();
  }
}
void SendMessage()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "**FALL ALERT**";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
void buzz(){
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(3000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
  
}


String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
