#include "GUI_interface_extend.h"
#include <wx/msgdlg.h>
#include "mathplot/mathplot.h"

// Local defines
#define MAX_NUM_POINTS      8
#define MAX_TEMPERATURE     300
#define MIN_TEMPERATURE     25

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

    // Add the basic layers to the graph

    m_plot = new mpWindow( this, -1, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER );
	m_plot->AddLayer( new mpScaleX( wxT("Elapsed time [s]")));
    m_plot->AddLayer( new mpScaleY( wxT("Temperature [°C]")));
	m_plot->EnableMousePanZoom(false);
	top_sizer->Add( m_plot, 2, wxEXPAND);
	this->Layout();
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
void GUI_frame_ext::insert_point( wxCommandEvent& event )
{
    long time, temperature;

    // Check if the max number of points hasn't been reached yet
    int num_items = this->points_list->GetItemCount();
    if ( num_items == MAX_NUM_POINTS ) {
        wxMessageBox( wxT("Max number of elements reached"), wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // Get data from input boxes
    if ( get_time_temp_data(time, temperature) < 0 ){
        return;
    }

    // Check that the specified temperature is inside allowed ranges
    if ( (temperature>MAX_TEMPERATURE)||(temperature<MIN_TEMPERATURE) ) {
        wxMessageBox( wxString::Format("Temperature out of range!\nValid range is between %d and %d", MIN_TEMPERATURE, MAX_TEMPERATURE),
                        wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // check for the right position in which the new point should be inserted
    int index = 0;
    wxListItem temp_item;
    long temp_time;

    for (index=0; index<num_items; index++){
        temp_item.SetId(index);
        temp_item.SetColumn(0);
        this->points_list->GetItem(temp_item);
        (temp_item.GetText()).ToLong(&temp_time);
        if (time == temp_time) {
            // If this time value already exist then exit
            wxMessageBox( wxT("This time value has already been specified"), wxT("Error"), wxICON_INFORMATION);
            return;
        }
        else if (time < temp_time){
            // Otherwise, if we found the right position then exit the from the "for" cycle
            break;
        }
    }

    // Convert numeric valuese into string again
    std::ostringstream time_text, temp_text;
    time_text << time;
    temp_text << temperature;

    // If we get here then the user specified valid data, so we can add them to our list
    points_list->InsertItem(index, time_text.str() );
    points_list->SetItem(index, 1, temp_text.str());

    // Refresh the graph
    this->update_graph();
}

/*
    Add a new point to the reflow list
*/
void GUI_frame_ext::clear_list( wxCommandEvent& event )
{
    points_list->DeleteAllItems();
}

/*
    Returns the time and temperature data contained into the text boxes (if valid)
*/
int GUI_frame_ext::get_time_temp_data(long& time, long& temperature)
{
    // Get the data from the text boxes
    wxString time_text_data = this->time_text->GetLineText(0);
    wxString temp_text_data = this->temp_text->GetLineText(0);

    // First check that the text boxes are not empty ...
    if ( time_text_data.length() == 0 ){
        wxMessageBox( wxT("Time text box is empty"), wxT("Error"), wxICON_INFORMATION);
        return -1;
    }
    if ( temp_text_data.length() == 0 ){
        wxMessageBox( wxT("Temperature text box is empty"), wxT("Error"), wxICON_INFORMATION);
        return -2;
    }

    // ... and contain valid data
    if( !time_text_data.ToLong(&time) ){
        wxMessageBox( wxT("Invalid time value"), wxT("Error"), wxICON_INFORMATION);
        return -3;
    }
    if( !temp_text_data.ToLong(&temperature) ){
        wxMessageBox( wxT("Invalid temperature value"), wxT("Error"), wxICON_INFORMATION);
        return -4;
    }
    return 0;
}

/*
    Remove the selected point from the list
*/
void GUI_frame_ext::remove_point( wxCommandEvent& event )
{
    // Check if the list is already empty or not
    if ( this->points_list->GetItemCount() == 0 ) {
        wxMessageBox( wxT("The list is already empty"), wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // Check that a line is selected from the points list
    if ( this->points_list->GetSelectedItemCount() == 0 ) {
        wxMessageBox( wxT("No line selected from the list"), wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // Get the index of the selected line
    int selected_item = this->points_list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    this->points_list->DeleteItem(selected_item);
}

/**
 *  Update graph with the specified points
 */
void GUI_frame_ext::update_graph()
{
	// retrieve the user selected points from the list
	std::vector<double> vectorx, vectory;
    int index = 0;
    int num_items = this->points_list->GetItemCount();
    wxListItem temp_item;
    double temp_value;

    for (index=0; index<num_items; index++){
        // Get the time
        temp_item.SetId(index);
        temp_item.SetColumn(0);
        this->points_list->GetItem(temp_item);
        (temp_item.GetText()).ToDouble(&temp_value);
        vectorx.push_back(temp_value);
        // Get the temperature
        temp_item.SetId(index);
        temp_item.SetColumn(1);
        this->points_list->GetItem(temp_item);
        (temp_item.GetText()).ToDouble(&temp_value);
        vectory.push_back(temp_value);
    }

    // Delete any previous plot layer (if any) before creating a new one
    mpLayer* old_layer = m_plot->GetLayerByName(wxString("selected_points"));
    if (old_layer != NULL){
        m_plot->DelLayer(old_layer, true, true);
    }

    // Create a mpFXYVector layer
	mpFXYVector* new_layer = new mpFXYVector(_("selected_points"));
	new_layer->SetData(vectorx, vectory);
	new_layer->SetContinuity(true);
	wxPen vectorpen(*wxBLUE, 1, wxSOLID);
	new_layer->SetPen(vectorpen);
	new_layer->SetDrawOutsideMargins(false);

    // Add this new layer to its mpWindow
	m_plot->AddLayer( new_layer );
	m_plot->Fit();
}

