//http://www.instructables.com/id/How-to-connect-Arduino-to-a-PC-through-the-serial-/

#include <iostream>
#include <SerialStream.h>
#include <SerialStreamBuf.h>
#include <SerialPort.h>
#include <string>

using namespace std;
using namespace LibSerial;

class Arduino{
public:
    Arduino();
    int open();
    DataBuffer read();
    void close();
private:
    string dev = "\\\\.\\COM6"; 	//"/dev/ACM0";
    SerialPort serial;
};


int main(int argc, char **argv)
{
    Arduino arduino();
    return 0;
}


Arduino::Arduino(){
    serial(dev);
}

int Arduino::abrir(){
    int estado = 0;

    serial.Open(SerialPort::BAUD_9600,
                SerialPort::CHAR_SIZE_8,
                SerialPort::PARITY_NONE,
                SerialPort::STOP_BITS_1,
                SerialPort::FLOW_CONTROL_NONE);

    if (serial.IsOpen() == false)
        estado = -1;

    return estado;
}

void Arduino::cerrar(){
    serial.Close();
}

DataBuffer Arduino::leer(){
    SerialPort::DataBuffer buffer;
    serial.Read(buffer, 10, 1000);

    return buffer;
}