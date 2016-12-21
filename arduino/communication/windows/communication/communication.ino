#include <Servo.h>

#define MSG_LEN 20

Servo servoShortEdge;
Servo servoLongEdge;

int inByte; //input byte
int degree=0;
int x,y,servo1,servo2;
char msgTx[MSG_LEN];  // #format: <x y servo1Degree servo2Degree> (int,int,int,int)
char moveRx,isGameRx;
char msgRx[MSG_LEN];  // #format: <setPointX setPointY> (int,int)
                      // move control input, no input is represented by 0
                      // isGame option activates game option
int iRx=0;


void setup() {
    Serial.begin(9600); // Seri iletişim ayarı - iletişimi aktifleştir ve hızını ayarla
    sprintf(msgTx, "");    //empty msg
    sprintf(msgRx, "");    //empty msg
    //servoShortEdge.attach(9);
    //servoLongEdge.attach(10);
    
    //reset();
}
 
void loop() {
  communicate();
}

void communicate(){
  /*if(rx() == 1){  //msg received
    sscanf(msgRx, "%c %c",moveRx,isGameRx);  //get values
    Serial.println(msgRx);
    //performRxCommand();
    clearRx();
  }*/
  tx();
}

void performRxCommand(){
  if(moveRx=='0'){      //no servo move
    
  }else{                //move servo
    if(moveRx==119) //'w' harfi int olarak 119'a karşılık geliyor, yani klavyeden 'w' değeri gönderilmişse
    {
      if(servoLongEdge.read()>10){
       servoLongEdge.write(servoLongEdge.read()-10);
      }
    }
    else if(moveRx==115)  //'s' harfi int olarak 115'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    {    
      if(servoLongEdge.read()<170){
       servoLongEdge.write(servoLongEdge.read()+10);
      }
    }
    else if(moveRx==97)  //'a' harfi int olarak 97'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    {    
      if(servoShortEdge.read()<170){
       servoShortEdge.write(servoShortEdge.read()+10);
      } 
    }
    else if(moveRx==100)  //'d' harfi int olarak 100'e karşılık geliyor, yani klavyeden 's' değeri gönderilmişse
    { 
      if(servoShortEdge.read()>10){
       servoShortEdge.write(servoShortEdge.read()-10);
      }   
    }
  }
  
  if(isGameRx == '1'){
    //do what you wanna do in the game
  }
}

int rx(){
  if (Serial.available() > 0) 
  { 
    inByte = Serial.read();
    if(inByte == '\0'){
      msgRx[iRx] = inByte;
      return 1;
    }else{
      msgRx[iRx++] = inByte;    //concat msg
    }
    if(iRx == MSG_LEN-1){
      msgRx[iRx] = '\0';
      return 1;
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
  Serial.write('\0');
  delay(1000);
  clearTx();
}

//TODO: NEED VARIABLES ASSIGNMENTS
void setStatus(){
  x = 1;                             //THESE SHOULD BE CORRECTLY SETTED
  y = 1;
  servo1 = 1; //servoShortEdge.read();    //THESE TWO LINES MAY BE EXCHANGED
  servo2 = 1; //servoLongEdge.read();
  //sprintf(msgTx, "%d %d %d %d", x,y,servo1,servo2);
  sprintf(msgTx, "A");
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
  iRx=0;
}

void clearTx(){
  sprintf(msgTx, "");
}

void reset(){
  servoLongEdge.write(75);
  servoShortEdge.write(75);
}

