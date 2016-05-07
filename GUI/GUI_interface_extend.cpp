#include "GUI_interface_extend.h"


/*
    Constructor
*/
GUI_frame_ext::GUI_frame_ext(wxWindow* parent) : GUI_frame( parent )
{
    device = new Communication();
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
    }
    this->m_choice1->SetSelection(0);
}

/*
    Try to connect to the selected COM port
*/
void GUI_frame_ext::connect_to_COM( wxCommandEvent& event )
{
    wxString portname = this->m_choice1->GetString(this->m_choice1->GetSelection());

    if( device->connect(portname) != 0 ) {
        wxMessageBox( wxT("Device not found"), wxT("Error"), wxICON_INFORMATION);
        this->status_bar->SetStatusText(wxString("Disconnected"));
    }else{
        this->status_bar->SetStatusText(wxString("Connected"));
    }
}
