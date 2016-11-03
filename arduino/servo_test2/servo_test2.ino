/*https://gelecegiyazanlar.turkcell.com.tr/konu/arduino/egitim/arduino-301/servo-motor
 *
 * Yazana göre servo sadece 9 ve 10 no'lu pinlerde çalışıyor (digital). 
 * https://www.arduino.cc/en/Reference/ServoAttach
 * http://playground.arduino.cc/Learning/SingleServoExample
*/
#include <Servo.h>  /* Servo kutuphanesi projeye dahil edildi */
Servo servoNesnesi;  /* servo motor nesnesi yaratildi */

void setup()
{
  servoNesnesi.attach(9);  /* Servo motor 9 numarali pine baglandi */
}
 
void loop()
{
  servoNesnesi.write(360);  /* Motorun mili 100. dereceye donuyor */
  delay(1000);
  servoNesnesi.write(0);   /* Motor mili 20. dereceye donuyor */
  delay(1000);
  servoNesnesi.detach();  /*bunu koymayınca alet durmuyor, reset butonu falan işlemiyor.*/
}
