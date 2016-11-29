/*=================================
This code demostrates 4-Wire Touch screen 
interfacing with Arduino

blog.circuits4you.com
www.circuits4you.com

4- Wire Touchscreen Connections
A0=====X+
A1=====X-
A2=====Y+
A3=====Y-
=================================*/
//Define your Touch screen connections
#define A0 X1
#define A1 X2
#define    Y1
#define A3 Y2
//Define your screen resolution as per your Touch screen (Max: 1024)
#define Xresolution 320 //128
#define Yresolution 240 //64

void setup()
{
   Serial.begin(9600);
}

void loop()
{
  int X,Y; //Touch Coordinates are stored in X,Y variable
   pinMode(A0,INPUT);
   pinMode(Y2,INPUT);  
   digitalWrite(Y2,LOW);
   pinMode(A0,OUTPUT);
   digitalWrite(A0,HIGH);
   pinMode(A1,OUTPUT);
   digitalWrite(A1,LOW);
   X = (analogRead(Y1))/(1024/Xresolution); //Reads X axis touch position
    
   pinMode(A0,INPUT);
   pinMode(A1,INPUT);
   digitalWrite(A1,LOW);
   pinMode(Y1,OUTPUT);
   digitalWrite(Y1,HIGH);
   pinMode(Y2,OUTPUT);
   digitalWrite(Y2,LOW);
   Y = (analogRead(A0))/(1024/Yresolution); //Reads Y axis touch position
  
  //Display X and Y on Serial Monitor
   Serial.print("X = ");  
   Serial.print(X);
   Serial.print(" Y = ");
   Serial.println(Y);
   delay(100);
}
