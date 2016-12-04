/////////Ball and Plate///////////////////////////////
/*
BALL AND PLATE PID CONTROL
*/
//////////////////////////////////////////////////////
///Libraries///
#include <stdint.h>
#include <SPI.h>
#include <Wire.h>
#include<Servo.h>
#include "PID_v1.h"
#include "TouchScreen.h"

// Definitions TOUCH PINS
#define YP A0 //0
#define XM A1 //1
#define YM 3  //3
#define XP 4 //4
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

unsigned int noTouchCount = 0; //viariable for noTouch TouchSreen

// servos variables
Servo servoX; //X axis
Servo servoY; //Y axis
int defaultAngle = 85; 

// Servo values
double OutputX; //for X
double OutputY; //for Y
double coff = 1.00;
//
      
int areaX = 1; //ServoX ye göre bölgeler
int areaY = 1; //ServoY ye göre bölgeler
int x , y;

unsigned long Stable=0;// sistem dengede mi? 

void setup()
{
  servoX.attach(9);
  servoY.attach(10);
  OutputX=defaultAngle;
  OutputY=defaultAngle;
  servoX.write(OutputX);
  servoY.write(OutputY);
  
  //INIT PINS
  //pinMode(8, OUTPUT);
  //pinMode(7, OUTPUT);
  //digitalWrite(8,LOW); //LED INIT
  //digitalWrite(7,LOW);

  Serial.begin(9600);
  
  //INIT OF TOUSCHSCREEN
  TSPoint p = ts.getPoint();
  
  //// Make plate flat
  servoX.attach(9); 
  servoY.attach(10);
  OutputX=defaultAngle;
  OutputY=defaultAngle;
  servoX.write(OutputX);
  servoY.write(OutputY);
   
  /////
  delay(100);
  ///
}
  
void loop() {
  while(Stable<100){ //REGULATION LOOP
    TSPoint p = ts.getPoint(); // measure actual position
    setPosition();  
    if (p.z >= 0) { //ball is on plate 
      
     //Serial.print("  Areas : ");  Serial.print(areaX);Serial.print(","); Serial.println(areaY);  
      servoX.attach(9); //connect servos
      //servoY.attach(10); 
      noTouchCount = 0;
       
      if(areaX == 0  && areaY == 0) { //if ball is close to setpoint
        //Serial.print("Outputs : ");  Serial.print(OutputX);   Serial.print(",");  Serial.print(OutputY);  Serial.print("\n");
          Stable=Stable+1; //increment STABLE
          coff = coff * 0.99;
          //digitalWrite(8,HIGH);           
      } else {
          //digitalWrite(8,LOW);
      } 
    } else { //if there is no ball on plate
      //noTouchCount++; //increment no touch count
      if(noTouchCount == 100) {
         //noTouchCount++; 
         OutputX=defaultAngle; 
         OutputY=defaultAngle;
         servoX.write(OutputX); 
         // servoY.write(OutputY);
        }
       if(noTouchCount == 200) //if there is no ball on plate longer
      {
         servoX.detach(); //detach servos
         //servoY.detach();     
      }
    }
     servoX.write(OutputX);//control
     //servoY.write(OutputY);//control
        
  } ////END OF REGULATION LOOP///
  
  servoX.detach();//detach servos
  //servoY.detach();
  
 ///CHECK STABILITY////
 while(Stable==100)//if is stable
 { //still measure actual postiion
    setPosition(); 
    if(areaX != 0  && areaY != 0) //if ball isnt close to setpoint
    {
      servoX.attach(9); //again attach servos
      //servoY.attach(10);
      //digitalWrite(8,LOW);
      Stable=0; //change STABLE state
    }
    
  }//end of STABLE LOOP
}//loop end

////////////////////////Functions////////////////// 
void setPosition(){
  setAreas();
  OutputX = (defaultAngle - ( areaX * 10  )) ;
  OutputY = (defaultAngle + ( areaY * 10  )) ;
  
}
void setAreas(){
  TSPoint p = ts.getPoint(); // measure actual position 
  x=p.x;
  y=p.y;
  
  Serial.print(x);
  Serial.print(" , ");
  Serial.print(y);
  Serial.print( "   ");

  x = map(x, 75, 500, 0, 200);
  y = map(y, 720, 1000, 0, 150); 

  Serial.print(x);
  Serial.print(" , ");
  Serial.print(y);
  Serial.print( "\n");
  
 
  if( x >= 0 && x <= 10 )
    areaX = 7;
  else if( x >= 10 && x <= 20 )
    areaX = 6;
  else if( x >= 20 && x <= 30 )
    areaX = 5;
  else if( x >= 30 && x <= 40)
    areaX = 4;
  else if( x >= 40 && x <= 50 )
    areaX = 3;
  else if( x >= 50 && x <= 60 )
    areaX = 2;
  else if( x >= 60 && x <= 70 )
    areaX = 1;
  else if( x >= 70 && x <= 80 )
    areaX = 0;
  else if( x >= 110 && x <= 122 )
    areaX = -1;
  else if( x >= 122 && x <= 134 )
    areaX = -2;
  else if( x >= 134 && x <= 146 )
    areaX = -3;
  else if( x >= 146 && x <= 158 )
    areaX = -4;
  else if( x >= 158 && x <= 170 )
    areaX = -5;
  else if( x >= 170 && x <= 185 )
    areaX = -6;
  else if( x >= 185 && x <= 200 )
    areaX = -7;


  
  if( y >= 0 && y <= 10 )
    areaY = 7; 
  if( y >= 10 && y <= 20 )
    areaY = 6; 
  if( y >= 20 && y <= 30 )
    areaY = 5; 
  if( y >= 30 && y <= 40 )
    areaY = 4; 
  if( y >= 40 && y <= 50 )
    areaY = 3; 
  if( y >= 50 && y <= 60 )
    areaY = 2; 
  if( y >= 60 && y <= 70 )
    areaY = 1; 
  if( y >= 70 && y <= 80 )
    areaY = 0; 
  if( y >= 80 && y <= 86 )
    areaY = -1;
  if( y >= 86 && y <= 92 )
    areaY = -2;
  if( y >= 92 && y <= 98 )
    areaY = -3;
  if( y >= 98 && y <= 104 )
    areaY = -4;
  if( y >= 104 && y <= 110 )
    areaY = -5;
  if( y >= 110 && y <= 116 )
    areaY = -6;
  if( y >= 116 && y <= 122 )
    areaY = -7;
  if( y >= 122 && y <= 128 )
    areaY = -8;
  if( y >= 128 && y <= 134 )
    areaY = -9;
    if( y >= 140 && y <= 150 )
    areaY = -10; 
    

  /*
    if( y >= 740 && y <= 760 )
    areaY = 7; 
  if( y >= 760 && y <= 780 )
    areaY = 6; 
  if( y >= 780 && y <= 800 )
    areaY = 5; 
  if( y >= 800 && y <= 800 )
    areaY = 4; 
  if( y >= 820 && y <= 820 )
    areaY = 3; 
  if( y >= 840 && y <= 860 )
    areaY = 2; 
  if( y >= 860 && y <= 870 )
    areaY = 1; 
  if( y >= 870 && y <= 900 )
    areaY = 0; 
  if( y >= 900 && y <= 910 )
    areaY = -1;
  if( y >= 900 && y <= 910 )
    areaY = -2;
  if( y >= 910 && y <= 920 )
    areaY = -3;
  if( y >= 920 && y <= 930 )
    areaY = -4;
  if( y >= 930 && y <= 940 )
    areaY = -5;
  if( y >= 940 && y <= 955 )
    areaY = -6;
  if( y >= 955 && y <= 990 )
    areaY = -7; 
  */
} 
 
