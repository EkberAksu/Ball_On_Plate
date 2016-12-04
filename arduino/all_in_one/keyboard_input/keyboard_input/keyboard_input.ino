#include <Servo.h>
#include "Keyboard.h"

Servo servoLongEdge;  // This servo used to touchscreen's long edge top or down movement
Servo servoShortEdge;  // This servo used to touchscreen's long edge top or down movement

  int i = 0;

void setup() {
  servoLongEdge.attach(10);  // attaches the servo on pin 9 to the servo object
  servoShortEdge.attach(9); // attaches the servo on pin 10 to the servo object
  Keyboard.begin();          // initialize control over the keyboard
}

void loop() {

 //fourCornerDemo();
 reset();
 enablePlateController();
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
  servoShortEdge.write(75);
}

/*
 * generate servo response from keyboard input
 * one keystroke equals one degree of movement in servo
 */
void enablePlateController(){
 char input = getInput();
 if(input=='w'){
   if(servoLongEdge.read()>0){
     servoLongEdge.write(servoLongEdge.read()-1);
   }
 }else if(input=='s'){
   if(servoLongEdge.read()<180){
     servoLongEdge.write(servoLongEdge.read()+1);
   }
 }else if(input=='a'){
   if(servoShortEdge.read()<180){
     servoShortEdge.write(servoShortEdge.read()+1);
   }
 }else if(input=='d'){
   if(servoShortEdge.read()>0){
     servoShortEdge.write(servoShortEdge.read()-1);
   }
 }
}

/*
 * take input from keyboard
 */
char getInput(){
  if (Serial.available() > 0) {
    // read incoming serial data:
    char inChar = Serial.read();
    // Type the next ASCII value from what you received:
    Keyboard.write(inChar);
    return inChar;
  }
}

/*
 * demo: four corners
 */
void fourCornerDemo(){
  servoLongEdge.write(0);
  servoShortEdge.write(0);
  delay(1000);
  

  servoLongEdge.write(0);
  servoShortEdge.write(160);
  delay(1000);

  servoLongEdge.write(160);
  servoShortEdge.write(0);
  delay(1000);

  servoLongEdge.write(160);
  servoShortEdge.write(160);
  delay(1000); 
}
