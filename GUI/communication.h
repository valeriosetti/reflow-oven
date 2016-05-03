#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "ctb-0.16/ctb.h"

class Communication : public ctb::SerialPort
{
    private:
        // Default parameters
        static const int baudrate;
        static const char* protocol;
        static const ctb::SerialPort::FlowControl flowControl;
        static const int timeout;

        // Private variables
        ctb::SerialPort* serialPort;
        bool connected;

    protected:

    public:
        Communication();
        ~Communication();
        int connect(const char* portname);
        void disconnect();
        int test_communication();
};

#endif // COMMUNICATION_H_INCLUDED
