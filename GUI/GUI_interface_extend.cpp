#include "GUI_interface_extend.h"
#include <wx/msgdlg.h>

/*
    Constructor
*/
GUI_frame_ext::GUI_frame_ext(wxWindow* parent) : GUI_frame( parent )
{
    device = new Communication();
    // Refresh the choiceBox with a list of available COM ports
    wxCommandEvent dummy;
    this->refresh_COM_list(dummy);
    // Add a text validator for input filtering
    this->time_text->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
    this->temp_text->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
    // Clear the reflow points' list and add the two basic columns
    this->points_list->ClearAll();
    this->points_list->AppendColumn(wxString(wxT("Time [s]")), wxLIST_FORMAT_CENTRE, wxLIST_AUTOSIZE_USEHEADER);
    this->points_list->AppendColumn(wxString(wxT("Temperature [°C]")), wxLIST_FORMAT_CENTRE, wxLIST_AUTOSIZE_USEHEADER);
    // Update the status bar
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

/*
    Add a new point to the reflow list
*/
void GUI_frame_ext::add_point( wxCommandEvent& event )
{
    // Get the data from the text boxes
    wxString time_text_data = this->time_text->GetLineText(0);
    wxString temp_text_data = this->temp_text->GetLineText(0);
    long time, temperature;

    // First check that the text boxes are not empty ...
    if ( time_text_data.length() == 0 ){
        wxMessageBox( wxT("Time text box is empty"), wxT("Error"), wxICON_INFORMATION);
        return;
    }
    if ( temp_text_data.length() == 0 ){
        wxMessageBox( wxT("Temperature text box is empty"), wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // ... and contain valid data
    if( !time_text_data.ToLong(&time) ){
        wxMessageBox( wxT("Invalid time value"), wxT("Error"), wxICON_INFORMATION);
        return;
    }
    if( !temp_text_data.ToLong(&temperature) ){
        wxMessageBox( wxT("Invalid temperature value"), wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // If we get here then the user specified valid data, so we can add them to our list
    long itemIndex = points_list->InsertItem(0, time_text_data);
    points_list->SetItem(itemIndex, 1, temp_text_data);
}

/*
    Add a new point to the reflow list
*/
void GUI_frame_ext::clear_list( wxCommandEvent& event )
{
    points_list->DeleteAllItems();
}
