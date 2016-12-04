#include <Servo.h>
Servo servoShortEdge;
Servo servoLongEdge;

int inByte; //input byte
int degree=0;


void setup() {
    Serial.begin(9600); // Seri iletişim ayarı - iletişimi aktifleştir ve hızını ayarla
    servoShortEdge.attach(9);
    servoLongEdge.attach(10);
    
    reset();
}
 
void loop() {
  enablePlateController();
}

void enablePlateController(){
  if (Serial.available() > 0) 
  { 
    inByte = Serial.read();
    Serial.write("input char:");
    Serial.println(inByte, DEC);
    if(inByte==119) //'w' harfi int olarak 119'a karşılık geliyor, yani klavyeden 'w' değeri gönderilmişse
    {
      if(servoLongEdge.read()>10){
       servoLongEdge.write(servoLongEdge.read()-10);
      }
      Serial.println(servoLongEdge.read());
    }
    else if(inByte==115)  //'s' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    {    
      if(servoLongEdge.read()<170){
       servoLongEdge.write(servoLongEdge.read()+10);
      }
      Serial.println(servoLongEdge.read());
    }
    else if(inByte==97)  //'a' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    {    
      if(servoShortEdge.read()<170){
       servoShortEdge.write(servoShortEdge.read()+10);
      } 
      Serial.println(servoShortEdge.read());
    }
    else if(inByte==100)  //'d' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    { 
      if(servoShortEdge.read()>10){
       servoShortEdge.write(servoShortEdge.read()-10);
      }   
      Serial.println(servoShortEdge.read());
    }
  }
}

void reset(){
  servoLongEdge.write(75);
  servoShortEdge.write(75);
}
