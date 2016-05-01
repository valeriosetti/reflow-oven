#include "GUI_interface_extend.h"
#include "stdint.h"

GUI_frame_ext::GUI_frame_ext(wxWindow* parent) : GUI_frame( parent )
{
    // Refresh the choiceBox with a list of available COM ports
    wxCommandEvent dummy;
    this->refresh_COM_list(dummy);
}

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
    const wxString prova = wxT("hello");
    for (uint16_t i=0; i<ports.size() ; i++){
        this->m_choice1->Append(ports[i]);
        this->m_choice1->SetSelection(0);
    }
}
