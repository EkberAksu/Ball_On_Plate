/*PROJE_1 COURSE - BALL AND PLATE PROJECT - GROUP 1*/
#include <stdint.h>
#include<Servo.h>
#include <Keypad.h>
#include "PID_v1.h"
#include "TouchScreen.h"

//Set Touch Pins
#define YP A0 //0
#define XM A1 //1
#define YM 3  //3
#define XP 4 //4
//Initialize Touchscreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// PID values
double SetpointX, InputX=200, OutputX; //for X
double SetpointY, InputY=150, OutputY; //for Y
// servos variables
Servo servoX; //X axis
Servo servoY; //Y axis

//Time sample
int Ts =20;
//Number of stable point situation 
unsigned long Stable=0; 
//PID const X
float KpX = 0.45;                                                     
float KiX = 0.01;                                                      
float KdX = 0.1;
//PID const Y
float KpY = 0.30;                                                       
float KiY = 0.02;                                                      
float KdY = 0.10;
long cas=0; 
//INIT PID
PID myPIDX(&InputX, &OutputX, &SetpointX, KpX, KiX, KdX, DIRECT);
PID myPIDY(&InputY, &OutputY, &SetpointY, KpY,KiY,KdY, DIRECT);

//Avarage arrays
int arrX[5] = {90,90,90,90,90};
int arrY[5] = {90,90,90,90,90};
int counter = 0;
//Communication
char msgTx[15];  // #format: <x y servo1Degree servo2Degree> (int,int,int,int)
char msgRx[15];  // #format: <move isGame> (char,char)
                 // move control input, no input is represented by 0
                 // isGame option activates game option
int iRx=0;
char inByte; //input byte
int gameChoice=1;

// Keypad Initialization
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte colPins[ROWS] = {25,24,23,22}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {29, 28, 27,26}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

byte rowPins2[ROWS] = {46,47,48,49};
byte colPins2[COLS] = {50,51,52,53};
Keypad keypad2 = Keypad( makeKeymap(keys),rowPins2,colPins2,ROWS,COLS);
int score[2]={0,0};

int kale2X=25,kale2Ya =110,kale2Yb=210;
int kale1X=375,kale1Ya =90,kale1Yb=190;

void setup()
{
  servoX.attach(9);
  servoY.attach(10);
  OutputX=90;
  OutputY=90;
  servoX.write(OutputX);
  servoY.write(OutputY);
  
  Serial.begin(9600);
  
  //INIT OF TOUSCHSCREEN
  TSPoint p = ts.getPoint();
  InputX=200;
  InputY=150;
  
  //INIT SETPOINT
  SetpointX=200;
  SetpointY=150;
  
  //Zapnutie PID
  myPIDX.SetMode(AUTOMATIC);
  myPIDX.SetOutputLimits(30, 150);
  myPIDY.SetMode(AUTOMATIC);
  myPIDY.SetOutputLimits(30, 150);
  
  //Set time sample
  myPIDY.SetSampleTime(Ts); 
  myPIDX.SetSampleTime(Ts);

  sprintf(msgTx, "");    //empty msg
  sprintf(msgRx, "");    //empty msg
 }
 
void loop()
{
if(gameChoice==1){
  while(Stable<100 && gameChoice == 1) //stable loop
  {
    TSPoint p = ts.getPoint();   //measure pressure on plate
    InputX  = map(p.x,115,960,0,400);
    InputY = map(p.y,150,910,0,300);
       
    if(p.x == 0|| p.y>1000){
      OutputX = 90;
      OutputY = 90;
    }
    else{
      myPIDX.Compute();  //action control X compute
      myPIDY.Compute();  //action control  Y compute  
      //Serial.print(InputX);   Serial.print(",");  Serial.print(InputY);  Serial.print("\n");

      OutputX = abs(180 - OutputX);
      delay(30);
    }
    if((InputX>SetpointX-4 && InputX<SetpointX+4 && InputY>SetpointY-3 && InputY<SetpointY+3)){ //if ball is close to setpoint
      Stable=Stable+1; //increment STABLE
    }
    arrX[counter] = OutputX;
    arrY[counter] = OutputY;
    if(counter==4)
      counter=0;
    else
      counter++;
    servoX.write(averageX());//control
    servoY.write(averageY());//control
    communicate();        
  }////END OF REGULATION LOOP/// 
  servoX.detach();//detach servos
  servoY.detach();
 ///Stability Controller////
 while(Stable==100)//if is stable
 {
    TSPoint p = ts.getPoint();
    InputX  = map(p.x,115,960,0,400);
    InputY = map(p.y,150,910,0,300); 
    if(InputX<SetpointX-4 || InputX>SetpointX+4 || InputY>SetpointY+3 || InputY<SetpointY-3  ) //if ball isnt close to setpoint
    {
      servoX.attach(9); //again attach servos
      servoY.attach(10);
      Stable=0; //change STABLE state
    }    
    communicate();  
  }//end of STABLE LOOP
}else{//FOOTBALL GAME
   servoX.attach(9); //again attach servos
   servoY.attach(10);
  //Serial.print("Girdi");
  TSPoint p = ts.getPoint();   //measure pressure on plate
    InputX  = map(p.x,115,960,0,400);
    InputY = map(p.y,150,910,0,300);
  if(InputX >= 200 && InputX>=0 && InputY >=0)
    plateMoveKeypad1();
  else if(InputX>=0 && InputY >=0)
    plateMoveKeypad2();   
  int res=0;  
  if( InputX>0 && InputY > 0 && (res = goal()) > 0){
    if(res == 2){
        servoX.write(servoX.read() - 20);
        delay(500);
        servoX.write(90);
    }else if(res == 1){
        servoX.write(servoX.read() + 20);
        delay(500);
        servoX.write(90);                
    }
    //delay(1000);
  }
  communicate();
}
  
}//loop end

/*-----------------------------FUNCTIONS---------------------------*/
int averageX(){
  return (arrX[0] + arrX[1] + arrX[2] + arrX[3] +arrX[4]) / 5;
}

int averageY(){
  return (arrY[0] + arrY[1] + arrY[2] + arrY[3] +arrY[4]) / 5;
}

void communicate(){
  int sX,sY;
  if(rx() == 1){  //msg received
    //Serial.print(msgRx);
    sscanf(msgRx, "%d,%d",&sX,&sY);  //get values
    if(sX == 999|| sY ==999){
      gameChoice=2;
    }else if(sX == 888|| sY ==888){
      SetpointX = 200.0;
      SetpointY = 150.0;
      gameChoice=1;
    }else{
      SetpointX = (double)sX;
      SetpointY = (double)sY;
    }
    clearRx();
  }
  if(InputX <0)
      InputX = 666;
    if(InputY <0)
      InputY = 666;
  tx();
}

int rx(){
  int k=0;
  if (Serial.available() > 0) 
  { while(k<8){
    inByte = Serial.read();
    //Serial.write(inByte);
    //Serial.write("\n");
    if((inByte )== 'x'){
      msgRx[iRx] = '\0';
      return 1;
    }else{
      msgRx[iRx++] = (inByte);    //concat msg
    }
    k++;
  }
  }
  return 0;
}

//writes msg string to serial port
void tx(){
  setStatus();
  for(int i=0; i<msgTxLength(); ++i){
    Serial.write(msgTx[i]);
  }
  clearTx();
}

//TODO: NEED VARIABLES ASSIGNMENTS
void setStatus(){
  sprintf(msgTx, "%03d,%03dx",(int)InputX,(int)InputY);
}

int msgTxLength(){
  int i;
  for(i=0; msgTx[i]!='\0'; ++i);
  return i;
}

int msgRxLength(){
  int i;
  for(i=0; msgRx[i]!='\0'; ++i);
  return i;
}

void clearRx(){
  sprintf(msgRx, "");
  //memset(msgRx,"",msgRxLength());
  iRx=0;
}

void clearTx(){
  sprintf(msgTx, "");
}
//FOOTBALL GAME
void plateMoveKeypad1()  {
  char key = keypad.getKey();
  if (key){ 
    switch (key)  {
       case '2':
       servoX.write(servoX.read() + 20);
             delay(500);
             servoX.write(90);            
              break;       
        case '4':
            servoY.write(servoY.read() + 20);
            delay(500);
            servoY.write(90); 
            break;
        case '5': 
           servoX.write(servoX.read() + 40);
           delay(500);
           servoX.write(90); 
           break;         
        case '6':
            servoY.write(servoY.read() - 20);
            delay(500);
            servoY.write(90); 
            break;
        case '8':
           servoX.write(servoX.read() - 20);
           delay(500);
           servoX.write(90); 
           break;  
      } 
    }
}

void plateMoveKeypad2()  {
   char key = keypad2.getKey();
  if (key){ 
    switch (key)  {
       case '2':
           servoX.write(servoX.read() - 20);
           delay(500);
           servoX.write(90);
           break;         
        case '4':
            servoY.write(servoY.read() - 20);
            delay(500);
            servoY.write(90); 
            break;
        case '5': 
           servoX.write(servoX.read() - 40);
           delay(500);
           servoX.write(90);
           break;         
        case '6':
              servoY.write(servoY.read() + 30);
              delay(500);
              servoY.write(90); 
              break;
        case '8':
             servoX.write(servoX.read() + 20);
             delay(500);
             servoX.write(90);           
              break;       
      } 
    }
}

int goal(){
  if(InputX > kale1X){
    if(InputY > kale1Ya && InputY <kale1Yb){
       
      score[0]++;
      return 1;
    }
  }else if(InputX < kale2X){
    if(InputY > kale2Ya && InputY <kale2Yb){
      
      score[1]++;
      return 2;
    }
  }else {
  
    return -1;
  }
}
