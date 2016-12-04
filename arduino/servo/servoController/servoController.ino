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
      if(servoLongEdge.read()>0){
       servoLongEdge.write(servoLongEdge.read()-10);
      }
      //degree++;
      //servoShort.write(75);
      Serial.println(servoLongEdge.read());
      //delay(1000);
      //if(motorSpeed <= 245) //Motor hızı 10 arttırıldığında 25        5'i aşmayacak ise
      //motorSpeed+=10;  //Motor hızını 10 arttır
      //analogWrite(9,motorSpeed);  //9.Pinde motor hızımız kadar PWM uygula
    }
    else if(inByte==115)  //'s' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    {    
      if(servoLongEdge.read()<180){
       servoLongEdge.write(servoLongEdge.read()+10);
      }
      //degree--;
      //servoShort.write(0);
      Serial.println(servoLongEdge.read());
      //delay(1000);
      //if(motorSpeed >= 10)  //Motor hızı 10 azaltığında 0'dan düşük bir duruma gelmeyecek ise
      //motorSpeed-=10;  //Motor hızını 10 azalt
      //analogWrite(9,motorSpeed);    //9.Pinde motor hızımız kadar PWM uygula
    }
    else if(inByte==97)  //'a' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    {    
      if(servoShortEdge.read()<180){
       servoShortEdge.write(servoShortEdge.read()+10);
      } 
      //degree--;
      //servoShort.write(0);
      Serial.println(servoShortEdge.read());
      //delay(1000);
      //if(motorSpeed >= 10)  //Motor hızı 10 azaltığında 0'dan düşük bir duruma gelmeyecek ise
      //motorSpeed-=10;  //Motor hızını 10 azalt
      //analogWrite(9,motorSpeed);    //9.Pinde motor hızımız kadar PWM uygula
    }
    else if(inByte==100)  //'d' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    { 
      if(servoShortEdge.read()<180){
       servoShortEdge.write(servoShortEdge.read()-10);
      }   
      //degree--;
      //servoShort.write(0);
      Serial.println(servoShortEdge.read());
      //delay(1000);
      //if(motorSpeed >= 10)  //Motor hızı 10 azaltığında 0'dan düşük bir duruma gelmeyecek ise
      //motorSpeed-=10;  //Motor hızını 10 azalt
      //analogWrite(9,motorSpeed);    //9.Pinde motor hızımız kadar PWM uygula
    }
  }
}

void reset(){
  servoLongEdge.write(75);
  servoShortEdge.write(75);
}
