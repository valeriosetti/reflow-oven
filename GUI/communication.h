#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#include "libctb-0.16/include/ctb.h"
#include <wx/msgdlg.h>
#include <wx/timer.h>

class Communication :  public ctb::SerialPort
{
    private:
        // Private variables
        bool connected;
        int send_command(std::string cmd);

    protected:

    public:
        Communication();
        ~Communication();
        int connect(const char* portname);
        void disconnect();
        int test_communication();
        int get_reflow_point(float& time, float& temperature);
        int add_reflow_point(wxString& time, wxString& temperature);
        int clear_reflow_list(void);
        int set_PID_parameters(unsigned int element, wxString& P_coeff, wxString& I_coeff, wxString& D_coeff);
        int start_reflow_process(void);
        int stop_reflow_process(void);
};

#endif // COMMUNICATION_H_INCLUDED
