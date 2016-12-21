
/**************************************************

file: demo_rx.c
purpose: simple demo that receives characters from
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_rx.c rs232.c -Wall -Wextra -o2 -o test_rx

received string format: <x y servo1Degree servo2Degree>

**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include "windows.h"
#else
#include <unistd.h>
#endif

#include "rs232.h"

#define MSG_LEN 20

void delay(int milliseconds);

int main()
{
  int n=0,
      cport_nr=5,        // change port no: http://www.teuniz.net/RS-232/
      bdrate=9600;       // 9600 baud
  //int iRx=0;

  unsigned char msgRx[MSG_LEN];
  //char c;

  char mode[]={'8','E','2',0};  // change mode http://www.teuniz.net/RS-232/


  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

  while(1)
  {
    n = RS232_PollComport(cport_nr, msgRx, MSG_LEN);
    delay(1000);
    printf("%d\n", n);

    if(n>0){
      for(int i=0; i<n; ++i){
        printf("<%d>", (int)msgRx[i]);
      }
      printf("\n");
      //printf("%c\n", c);
      /*if(c == 0){
        msgRx[iRx] = '\0';
        printf("received %i bytes: <%s>\n", n, (char *)msgRx);
        sprintf(msgRx, "");
      }else{
        msgRx[iRx++] = c;
      }*/

      if(n >= MSG_LEN-1){
        msgRx[MSG_LEN-1] = '\0';
      }else{
        msgRx[n] = '\0';
        printf("received %i bytes: <%s>\n", n, (char *)msgRx);
      }

      for(int i=0; i<n; ++i) msgRx[i] = '\0';
    }

#ifdef _WIN32
    Sleep(100);
#else
    usleep(100000);  /* sleep for 100 milliSeconds */
#endif
  }

  return(0);
}


void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

