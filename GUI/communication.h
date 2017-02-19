#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "libctb-0.16/include/ctb.h"
#include <wx/msgdlg.h>
#include <wx/timer.h>
#include "stdint.h"

class Communication :  public ctb::SerialPort
{
    private:
        // Private variables
        bool connected;
        int send_command(std::stringstream& cmd);

    protected:

    public:
        Communication();
        ~Communication();
        int connect(const char* portname);
        bool connection_status(void);
        void disconnect();
        int test_communication();
        int get_reflow_point(float& time, float& temperature);
        int add_reflow_point(wxString& time, wxString& temperature);
        int clear_reflow_list(void);
        int set_PID_parameters(unsigned int element, wxString& P_coeff, wxString& I_coeff, wxString& D_coeff);
        int start_reflow_process(void);
        int stop_reflow_process(void);
        int set_reflow_process_period(wxString& period);
        int get_reflow_process_data(uint32_t* tick, uint32_t* target_temp, uint32_t* thermo1, uint32_t* thermo2, uint32_t* duty1, uint32_t* duty2);
};

#endif // COMMUNICATION_H_INCLUDED
