
#include "communication.h"
#include <sstream>

// Set this to TRUE for debug purposes. TX and RX lines MUST be shorted
// in order to loopback all commands
#define SERIAL_LOOPBACK             FALSE

// Virtual serial port defines
#define SERIAL_PORT_BAUDRATE        115200
#define SERIAL_PROTOCOL             "8N1"
#define SERIAL_FLOW_CONTROL         ctb::SerialPort::NoFlowControl
#define SERIAL_TIMEOUT              2000    // milliseconds
#define SERIAL_EOS_CHAR             (char*)"\n"
// Reflow process defines
#define REFLOW_SCAN_TIMEOUT     2000    // milliseconds. It's the maximum interval between STM32's temperature redings before declaring a timeout

// Macros
#define TEST(_expr_, _errno_)    if(_expr_) { this->disconnect(); return (_errno_); }

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
    TEST(this->Open(portname, SERIAL_PORT_BAUDRATE, SERIAL_PROTOCOL, SERIAL_FLOW_CONTROL) == -1, -1);
    connected = true;
    // Send a test packet to verify that there's the STM32 microcontroller on the other side of the link
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    TEST(this->test_communication() != 0, -2);
    return 0;
}

/**
 *  @brief  Disconnect from the targer
 *  @param  [none]
 */
void Communication::disconnect()
{
    this->Close();
    connected = false;
}

/**
 *  @brief  Returns the current status of the connection
 *  @param  [none]
 *  @return True if the connection is established correctly; false otherwise.
 */
bool Communication::connection_status(void)
{
    return connected;
}

/**
 *  @brief  Send a test packet to the microcontroller and wait for its answer. If no answer
 *          is received before the timeout then an error is returned
 *  @param  [none]
 */
int Communication::test_communication()
{
    std::stringstream cmd;
    cmd << "test";
    return this->send_command(cmd);
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
    cmd << "add_reflow_point " << time << " " << temperature;
    return this->send_command(cmd);
}

int Communication::clear_reflow_list()
{
    std::stringstream cmd;
    cmd << "clear_reflow_list";
    return this->send_command(cmd);
}

int Communication::set_PID_parameters(unsigned int element, wxString& P_coeff, wxString& I_coeff, wxString& D_coeff)
{
    std::stringstream cmd;
    cmd << "set_PID_parameters " << element << " " << P_coeff << " " << I_coeff << " " << D_coeff;
    return this->send_command(cmd);
}

int Communication::start_reflow_process()
{
    std::stringstream cmd;
    cmd << "start_reflow_process";
    return this->send_command(cmd);
}

int Communication::stop_reflow_process()
{
    std::stringstream cmd;
    cmd << "stop_reflow_process";
    return this->send_command(cmd);
}

int Communication::set_reflow_process_period(wxString& period)
{
    std::stringstream cmd;
    cmd << "set_reflow_process_period " << period;
    return this->send_command(cmd);
}

int Communication::get_reflow_process_data(uint32_t* tick, uint32_t* target_temp, uint32_t* thermo1, uint32_t* thermo2)
{
    int ret_val;
    char* ret_string;
    size_t readed_bytes;

    // See if there's any data coming from the STM32
    ret_val = this->ReadUntilEOS(ret_string, &readed_bytes, SERIAL_EOS_CHAR, SERIAL_TIMEOUT);
    TEST(ret_val == 0, -1); // Timeout
    // Check if the process stop for some reason
    TEST(strcmp(ret_string,"Stop") == 0, 1);   // End of the job (return a positive value because that's not an error)
    TEST(strcmp(ret_string,"Error") == 0, -2);  // There was an error inside the STM32

    sscanf(ret_string, "time %d target %d temp_1 %d temp_2 %d", tick, target_temp, thermo1, thermo2);
    return 0;
}

int Communication::send_command(std::stringstream& cmd)
{
    int ret_val;
    char* ret_string;

    // If there's not active connection, then return immediately with an error
    TEST(connected == false, -1);
    // In case of loopback mode, keep track of the original command (without "\n")
    #if (SERIAL_LOOPBACK==TRUE)
        std::stringstream reference_cmd;
        reference_cmd << cmd.rdbuf();
    #endif
    // Add the new line char to the command
    cmd << "\n";
    // Try to send the command
    ret_val = this->Writev((char*)cmd.str().c_str(), cmd.str().length(), SERIAL_TIMEOUT);
    // If the sent data was less than expected then return error
    TEST((unsigned)ret_val != cmd.str().length(), -2);
    // Check if something was returned from the microcontroller
    size_t readed_bytes;
    ret_val = this->ReadUntilEOS(ret_string, &readed_bytes, SERIAL_EOS_CHAR, SERIAL_TIMEOUT);
    TEST(ret_val == 0, -3);

    #if (SERIAL_LOOPBACK==TRUE)
        TEST(strcmp(ret_string,(char*)reference_cmd.str().c_str()) != 0, -4);
    #else
        TEST(strcmp(ret_string,"OK") != 0, -4);
    #endif

    return 0;
}
