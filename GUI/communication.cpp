
#include "communication.h"
#include <sstream>

// Virtual serial port defines
#define SERIAL_PORT_BAUDRATE        115200
#define SERIAL_PROTOCOL             "8N1"
#define SERIAL_FLOW_CONTROL         ctb::SerialPort::NoFlowControl
#define SERIAL_TIMEOUT              2000    // milliseconds
#define SERIAL_EOS_CHAR             (char*)"\n"
// Reflow process defines
#define REFLOW_SCAN_TIMEOUT     2000    // milliseconds. It's the maximum interval between STM32's temperature redings before declaring a timeout


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
    int ret = this->Open(portname, SERIAL_PORT_BAUDRATE, SERIAL_PROTOCOL, SERIAL_FLOW_CONTROL);

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
    std::stringstream cmd;
    cmd << "test\n";
    return this->send_command(cmd.str());
}

int Communication::get_reflow_point(float& time, float& temperature)
{
    time = 0.0;
    temperature = 0.0;
    return 0;
}

int Communication::add_reflow_point(wxString& time, wxString& temperature)
{
    std::stringstream cmd;
    cmd << "add_reflow_point " << time << " " << temperature << "\n";
    return this->send_command(cmd.str());
}

int Communication::clear_reflow_list()
{
    std::stringstream cmd;
    cmd << "clear_reflow_list\n";
    return this->send_command(cmd.str());
}

int Communication::set_PID_parameters(unsigned int element, wxString& P_coeff, wxString& I_coeff, wxString& D_coeff)
{
    std::stringstream cmd;
    cmd << "set_PID_parameters " << element << " " << P_coeff << " " << I_coeff << " " << D_coeff << "\n";
    return this->send_command(cmd.str());
}

int Communication::start_reflow_process()
{
    std::stringstream cmd;
    cmd << "start_reflow_process\n";
    return this->send_command(cmd.str());
}

int Communication::stop_reflow_process()
{
    std::stringstream cmd;
    cmd << "stop_reflow_process\n";
    return this->send_command(cmd.str());
}

int Communication::set_reflow_process_period(wxString& period)
{
    std::stringstream cmd;
    cmd << "set_reflow_process_period " << period << "\n";
    return this->send_command(cmd.str());
}

int Communication::send_command(std::string cmd)
{
    int ret_val;
    char* ret_string;

    // Try to send the command
    ret_val = this->Writev((char*)cmd.c_str(), cmd.length(), SERIAL_TIMEOUT);
    if ( (unsigned)ret_val != cmd.length()){
        // If the sent data was less than expected then return error
        this->disconnect();
        return -1;
    }

    // Check if something was returned from the microcontroller
    size_t readed_bytes;
    ret_val = this->ReadUntilEOS(ret_string, &readed_bytes, SERIAL_EOS_CHAR, SERIAL_TIMEOUT);
    if ( ret_val == 0){
        // Nothing was returned before the timeout, so return an error
        this->disconnect();
        return -2;
    }

    if ( strcmp(ret_string,"OK") != 0 ){
        this->disconnect();
        return -3;
    }

    return 0;
}
