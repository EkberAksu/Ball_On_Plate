#include <Servo.h>  /* Servo kutuphanesi projeye dahil edildi */
Servo servoShort;  /* servo motor nesnesi yaratildi */

int inByte; //Seri yoldan gelen veri
int motorSpeed=125; //İlk durumda motorun çalışma hızı
int degree=0;


void setup() {
    Serial.begin(9600); // Seri iletişim ayarı - iletişimi aktifleştir ve hızını ayarla
    servoShort.attach(9);
    //analogWrite(9,motorSpeed);  //Motorumuzu çalıştır
}
 
void loop() {  //Program boyunca çalışan döngü
  if (Serial.available() > 0) 
  { 
    //Eğer seri bilgi gelmişse
    inByte = Serial.read(); //O bilgiyi oku ve inByte değişkenine ata
    Serial.write("Alinan karakter:"); //Bilgisayara seri olarak 'Alınan Karakter:' yazısı gönder
    Serial.println(inByte, DEC);  //Bilgisayara seri olarak inByte değişkenindeki değeri onluk tabanda  gönder(bildiğiniz 10)
    if(inByte==119) //'w' harfi int olarak 119'a karşılık geliyor, yani klavyeden 'w' değeri gönderilmişse
    {
      degree++;
      servoShort.write(75);
      Serial.println(degree, DEC);
      delay(1000);
      //if(motorSpeed <= 245) //Motor hızı 10 arttırıldığında 25        5'i aşmayacak ise
      //motorSpeed+=10;  //Motor hızını 10 arttır
      //analogWrite(9,motorSpeed);  //9.Pinde motor hızımız kadar PWM uygula
    }
    if(inByte==115)  //'s' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    {    
      degree--;
      servoShort.write(0);
      Serial.println(degree, DEC);
      delay(1000);
      //if(motorSpeed >= 10)  //Motor hızı 10 azaltığında 0'dan düşük bir duruma gelmeyecek ise
      //motorSpeed-=10;  //Motor hızını 10 azalt
      //analogWrite(9,motorSpeed);    //9.Pinde motor hızımız kadar PWM uygula
    }
  }
}
