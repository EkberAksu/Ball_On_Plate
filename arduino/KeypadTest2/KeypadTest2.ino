/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>
#include<Servo.h>

Servo servoX; //X axis
Servo servoY; //Y axis

// Keypad Initialization
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte colPins[ROWS] = {26, 27,28, 29}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {22, 23, 24,25}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

byte rowPins2[ROWS] = {37,36,35,34};
byte colPins2[COLS] = {33,32,31,30};
Keypad keypad2 = Keypad( makeKeymap(keys),rowPins2,colPins2,ROWS,COLS);

void setup(){
  Serial.begin(9600);
  servoX.attach(9);
  servoY.attach(10);
  servoX.write(90);
  servoY.write(90);
}
  
void loop(){
  plateMoveKeypad2();
}

void plateMoveKeypad1()  {
  char key = keypad.getKey();
  
  if (key){ 
    switch (key)  {
       case '1' :
            if(servoY.read()<170)
                 servoY.write(servoY.read()+10);
            if(servoX.read()<170)
                   servoX.write(servoX.read()+10);
          break;
       case '2':
              if(servoX.read()>10)
                  servoX.write(servoX.read()-10);
           break;
       case '3':
            if(servoY.read()>10)
                servoY.write(servoY.read()-10);
            if(servoX.read()<170)
                servoX.write(servoX.read()+10);
          break;
        case '4':
            if(servoY.read()>10)
                 servoY.write(servoY.read()-10);    
            break;
        case '5': 
                servoX.write(90);
                servoY.write(90);
                break;
        case '6':
              if(servoY.read()<170)
                 servoY.write(servoY.read()+10);
              break;
         case '7':
              if(servoY.read()<170)
                 servoY.write(servoY.read()+10);
              if(servoX.read()>10)
                  servoX.write(servoX.read()-10);
              break;   
        case '8':
             if(servoX.read()<170)
                   servoX.write(servoX.read()+10);    
              break;     
        case '9':
             if(servoY.read()>10)
                 servoY.write(servoY.read()-10);
             if(servoX.read()>10)
                servoX.write(servoX.read()-10);
              break;  
      } 
    }
}

void plateMoveKeypad2()  {
   char key = keypad2.getKey();
  
  if (key){ 
    switch (key)  {
       case '1' :
             if(servoY.read()>10)
                 servoY.write(servoY.read()-10);
             if(servoX.read()>10)
                servoX.write(servoX.read()-10);
          break;
       case '2':
              if(servoX.read()>10)
                  servoX.write(servoX.read()-10);  
           break;
       case '3':
              if(servoY.read()<170)
                 servoY.write(servoY.read()+10);
              if(servoX.read()>10)
                  servoX.write(servoX.read()-10);
          break;
        case '4':
            if(servoY.read()>10)
                 servoY.write(servoY.read()-10);    
            break;
        case '5': 
                servoX.write(90);
                servoY.write(90);
                break;
        case '6':
              if(servoY.read()<170)
                 servoY.write(servoY.read()+10);
              break;
         case '7':
            if(servoY.read()>10)
                servoY.write(servoY.read()-10);
            if(servoX.read()<170)
                servoX.write(servoX.read()+10);
              break;   
        case '8':
              if(servoX.read()<170)
                   servoX.write(servoX.read()+10);    
              if(servoX.read()>10)
                  servoX.write(servoX.read()-10);
              break;     
        case '9':
              if(servoY.read()<170)
                 servoY.write(servoY.read()+10);
            if(servoX.read()<170)
                   servoX.write(servoX.read()+10);
              break;  
      } 
    }
}
