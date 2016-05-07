
#include "communication.h"

// Define some basic communication parameters
const int Communication::baudrate = 115200;
const char* Communication::protocol = "8N1";
const ctb::SerialPort::FlowControl Communication::flowControl = ctb::SerialPort::NoFlowControl;
const int Communication::timeout = 1000;

/**
 *  @brief  Constructor
 *  @param  [none]
 */
Communication::Communication()
{
    connected = false;
}

/**
 *  @brief Desctructor
 *  @param  [none]
 */
Communication::~Communication()
{

}

/**
 *  @brief     Try to connect to a specified COM port.
 *  @param[in] portname => is the string specifying the selected port
 *  @return    0 if success; negative values otherwise
 */
int Communication::connect(const char* portname)
{
    // Try to open the selected COM port
    int ret = this->Open(portname, this->baudrate, this->protocol, this->flowControl);

    if (ret == 0){
        // In case it fails return an error
        connected = false;
        return -1;
    }

    // Send a test packet to verify that there's the STM32 microcontroller on the other
    // side of the link
    if (this->test_communication() != 0){
        // In case it fails return an error
        connected = false;
        return -2;
    }

    connected = true;
    return 0;
}

/**
 *  @brief  Disconnect from the targer
 *  @param  [none]
 */
void Communication::disconnect()
{
    connected = false;
}

/**
 *  @brief  Send a test packet to the microcontroller and wait for its answer. If no answer
 *          is received before the timeout then an error is returned
 *  @param  [none]
 */
int Communication::test_communication()
{
    // Define Tx and Rx test buffers
    std::string test_string("test\n");
    char* ret_string;
    int ret_val;

    // Try to send test_string
    ret_val = this->Writev((char*)test_string.c_str(), test_string.length(), this->timeout);
    if ( (unsigned)ret_val != test_string.length()){
        // If the sent data was less than expected then return error
        this->disconnect();
        return -2;
    }

    // Test if something was returned from the microcontroller
//    ret_val = this->Readv(receiveBuf, sizeof(receiveBuf)-1, this->timeout);
    size_t readed_bytes;

    ret_val = this->ReadUntilEOS(ret_string, &readed_bytes);
    if ( ret_val == 0){
        // Nothing was returned before the timeout, so return an error
        this->disconnect();
        return -3;
    }

    if ( strcmp(ret_string,"OK") != 0 ){
        this->disconnect();
        return -4;
    }

    return 0;
}
