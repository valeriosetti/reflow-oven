#include "GUI_interface_extend.h"
#include "stdint.h"
#include <string.h>
#include <wx/msgdlg.h>

// Define some basic communication parameters
const int GUI_frame_ext::baudrate = 115200;
const char* GUI_frame_ext::protocol = "8N1";
const ctb::SerialPort::FlowControl GUI_frame_ext::flowControl = ctb::SerialPort::NoFlowControl;

/*
    Constructor
*/
GUI_frame_ext::GUI_frame_ext(wxWindow* parent) : GUI_frame( parent )
{
    COM_device = NULL;
    // Refresh the choiceBox with a list of available COM ports
    wxCommandEvent dummy;
    this->refresh_COM_list(dummy);
    this->status_bar->SetStatusText(wxString("Disconnected"));
}

/*
    Destructor
*/
GUI_frame_ext::~GUI_frame_ext()
{

}

/*
    Refresh the list available COM ports
*/
void GUI_frame_ext::refresh_COM_list( wxCommandEvent& event )
{
    // Clear the current list
    this->m_choice1->Clear();

    // Get a list of availablt COM ports
    std::vector<std::string> ports;
    ctb::GetAvailablePorts( ports );

    // Add them to the ChoiceBox
    for (uint16_t i=0; i<ports.size() ; i++){
        this->m_choice1->Append(ports[i]);
        this->m_choice1->SetSelection(0);
    }
}

/*
    Try to connect to the selected COM port
*/
void GUI_frame_ext::connect_to_COM( wxCommandEvent& event )
{
    ctb::SerialPort* serialPort = new ctb::SerialPort();
    wxString devname = this->m_choice1->GetString(this->m_choice1->GetSelection());

    if( serialPort->Open( devname.c_str(), this->baudrate, this->protocol, this->flowControl ) == 0 ) {
        wxMessageBox( wxT("Unable to open port"), wxT("Error"), wxICON_INFORMATION);
        COM_device = serialPort;
        return;
    }else{
        this->status_bar->SetStatusText(wxString("Connected"));
    }
}
