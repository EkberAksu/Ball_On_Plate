#include <Servo.h>

Servo servoLongEdge;  // This servo used to touchscreen's long edge top or down movement
Servo servoShortEdge;  // This servo used to touchscreen's long edge top or down movement

  int i = 0;

void setup() {
  servoLongEdge.attach(10);  // attaches the servo on pin 9 to the servo object
  servoShortEdge.attach(9); // attaches the servo on pin 10 to the servo object
}

void loop() {

 // fourCornerDemo();
 reset();
}

/*
 * Set two servos to specific angle
 * l: Long edge angle
 * s: Short edge angle
 */
void setServoLongShort(int l, int s){
  servoLongEdge.write(l);
  servoShortEdge.write(s);
}  

/*
 * Reset plate
 */
void reset(){
  servoLongEdge.write(75);
  servoShortEdge.write(80);
}

void fourCornerDemo(){
  servoLongEdge.write(0);
  servoShortEdge.write(0);
  delay(1000);
  

  servoLongEdge.write(0);
  servoShortEdge.write(170);
  delay(1000);

  servoLongEdge.write(170);
  servoShortEdge.write(0);
  delay(1000);

  servoLongEdge.write(170);
  servoShortEdge.write(170);
  delay(1000); 
}

