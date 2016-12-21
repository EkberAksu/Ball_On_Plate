
/**************************************************

file: demo_tx.c
purpose: simple demo that transmits characters to
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_tx.c rs232.c -Wall -Wextra -o2 -o test_tx

Transmitted string format: <setPointX setPointY>

**************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include "windows.h"
#else
#include <unistd.h>
#endif

#include "rs232.h"

#define MSG_LEN 20

int main()
{
  int cport_nr=5,        // change port no: http://www.teuniz.net/RS-232/
      bdrate=9600;       // 9600 baud
  int setPointX, setPointY;

  char mode[]={'8','E','2',0};  // change mode http://www.teuniz.net/RS-232/
  char msgTx[MSG_LEN];

  setPointX = 200;
  setPointY = 150;        //center
  sprintf(msgTx, "%d %d", setPointX, setPointY);

  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

  while(1)
  {
    RS232_cputs(cport_nr, msgTx);

    printf("sent: %s\n", msgTx);

#ifdef _WIN32
    Sleep(1000);
#else
    usleep(1000000);  /* sleep for 1 Second */
#endif

  }

  return(0);
}

