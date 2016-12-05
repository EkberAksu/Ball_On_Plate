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
double xVal   , yVal;
/////TIME SAMPLE
int Ts = 50; 
int servoMax = 180;
int servoMin = 0;

unsigned int noTouchCount = 0; //viariable for noTouch TouchSreen
// Servo values
double OutputX; //for X
double OutputY; //for Y

// PID values
double SetpointX = 95; //for X
double SetpointY = 70; //for Y
//PID const
float Kp = 0.3;                                                     
float Ki = 0.03;                                                      
float Kd = 0.13;

float Kp1 = 0.3;                                                       
float Ki1 = 0.08;                                                      
float Kd1 = 0.13;
long cas=0;
 
//INIT PID
PID myPIDX(&xVal, &OutputX, &SetpointX, Kp, Ki, Kd, DIRECT);
PID myPIDY(&yVal, &OutputY, &SetpointY,Kp1,Ki1,Kd1, DIRECT);

// servos variables
Servo servoX; //X axis
Servo servoY; //Y axis
int defaultAngle = 90; 


//
      


unsigned long Stable=0;// sistem dengede mi? 

void setup()
{
  Serial.begin(9600);
  
  //INIT OF TOUSCHSCREEN
  TSPoint p = ts.getPoint();
  
  //// Make plate flat
  servoX.attach(10); 
  servoY.attach(9);
  OutputX=defaultAngle;
  OutputY=defaultAngle;
  servoX.write(OutputX);
  servoY.write(OutputY);

  //Zapnutie PID
  myPIDX.SetMode(AUTOMATIC);
  myPIDX.SetOutputLimits(00, 180);
  myPIDY.SetMode(AUTOMATIC);
  myPIDY.SetOutputLimits(00, 180);

    // TIME SAMPLE
  //myPIDY.SetSampleTime(Ts); 
  //myPIDX.SetSampleTime(Ts);  
  /////
  /////
  delay(100);
  ///
}
  
void loop() {
    TSPoint p = ts.getPoint(); // measure actual position
    
    xVal = map(p.x,115,960,0,200 );
    yVal = map(p.y,150,910,0,150 );
    
    if(xVal >= 0 && yVal >=0 ){//Baınç yoksa çalışmaz
    //setPosition();  
      servoX.attach(10); //connect servos
      servoY.attach(9); 
      //noTouchCount = 0; 
    OutputX =  (yVal * (1.05)); 
    OutputX = abs( 180 - OutputX ); //Uzun kenar motorun kuvveti
    
    OutputY = (xVal * (0.7));//Kısa kenar motorun kuvveti
    /*
   OutputX = map(OutputX,0,180,servoMin,servoMax );
    OutputY = map(OutputY,0,180,servoMin,servoMax );
    servoMin++;
    servoMax--;
    if((servoMax<105 && servoMin>75)){
      servoMax = 180;
      servoMin = 0;
     
    }
*/
    //myPIDX.Compute();
    //myPIDY.Compute();
  
    servoX.write(OutputX);//control
    servoY.write(OutputY);//control
    }
    //Serial.print(p.z); 
    Serial.print("X");  Serial.print(xVal);Serial.print(", Y "); Serial.print(yVal); Serial.print("\n ");
    //Serial.print("ServoUzun : "); Serial.println(OutputX); Serial.print(",servoKısa : "); Serial.println(OutputY);
  ////END OF REGULATION LOOP///
  
 //servoX.detach();//detach servos
  //servoY.detach();
  
 ///CHECK STABILITY////
 
 //end of STABLE LOOP
}//loop end

////////////////////////Functions////////////////// 
 
