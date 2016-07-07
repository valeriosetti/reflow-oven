#include "GUI_interface_extend.h"
#include <wx/msgdlg.h>
#include "mathplot/mathplot.h"

// Local defines
#define MAX_NUM_POINTS      8

#define MAX_TEMPERATURE     300
#define MIN_TEMPERATURE     25
#define TEMP_INTERVAL       1

#define TIME_INTERVAL       5
#define MIN_TIME            0+TIME_INTERVAL
#define MAX_TIME            1200

#define COEFFS_INTERVAL     0.1
#define COEFFS_MIN_VALUE    0
#define COEFFS_MAX_VALUE    10

/*
    Constructor
*/
GUI_frame_ext::GUI_frame_ext(wxWindow* parent)
                : GUI_frame( parent )
{
    STM32_device = new Communication();
    // Refresh the choiceBox with a list of available COM ports
    wxCommandEvent dummy;
    this->refresh_COM_list(dummy);
    // Clear the reflow points' list and add the two basic columns
    this->points_list->ClearAll();
    this->points_list->AppendColumn(wxString(wxT("Time [s]")), wxLIST_FORMAT_CENTRE, wxLIST_AUTOSIZE_USEHEADER);
    this->points_list->AppendColumn(wxString(wxT("Temperature [°C]")), wxLIST_FORMAT_CENTRE, wxLIST_AUTOSIZE_USEHEADER);

    // Add the reflow starting point by default
    this->points_list->InsertItem(0, wxT("0") );
    this->points_list->SetItem(0, 1, wxT("25"));

    // Update the status bar
    this->status_bar->SetStatusText(wxString("Disconnected"));

    // Set allowed temp values into the proper choicebox
    for (int temp_val=MIN_TEMPERATURE; temp_val<=MAX_TEMPERATURE; temp_val+=TEMP_INTERVAL) {
        this->temp_choice->Append( wxString::Format(wxT("%d"), temp_val) );
    }
    this->temp_choice->SetSelection(0);

    // Set allowed time values into the proper choicebox
    for (int time_val=MIN_TIME; time_val<=MAX_TIME; time_val+=TIME_INTERVAL) {
        this->time_choice->Append( wxString::Format(wxT("%d"), time_val) );
    }
    this->time_choice->SetSelection(0);

    // Set predefined P-I-D values
    for (float coeffs=COEFFS_MIN_VALUE; coeffs<=COEFFS_MAX_VALUE; coeffs+=COEFFS_INTERVAL) {
        this->P_choice_1->Append( wxString::Format(wxT("%.3f"), coeffs) );
        this->I_choice_1->Append( wxString::Format(wxT("%.3f"), coeffs) );
        this->D_choice_1->Append( wxString::Format(wxT("%.3f"), coeffs) );
        this->P_choice_2->Append( wxString::Format(wxT("%.3f"), coeffs) );
        this->I_choice_2->Append( wxString::Format(wxT("%.3f"), coeffs) );
        this->D_choice_2->Append( wxString::Format(wxT("%.3f"), coeffs) );
    }
    this->P_choice_1->SetSelection(0);
    this->I_choice_1->SetSelection(0);
    this->D_choice_1->SetSelection(0);
    this->P_choice_2->SetSelection(0);
    this->I_choice_2->SetSelection(0);
    this->D_choice_2->SetSelection(0);

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

    if( STM32_device->connect(portname) != 0 ) {
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

    // Get the data from the text boxes
    wxString time_text = this->time_choice->GetString(this->time_choice->GetSelection());
    time_text.ToLong(&time);
    wxString temp_text = this->temp_choice->GetString(this->temp_choice->GetSelection());
    temp_text.ToLong(&temperature);

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

    // If we get here then the user specified valid data, so we can add them to our list
    this->points_list->InsertItem(index, time_text );
    this->points_list->SetItem(index, 1, temp_text );

    // Refresh the graph
    this->update_graph();
}

/*
    Add a new point to the reflow list
*/
void GUI_frame_ext::clear_list( wxCommandEvent& event )
{
    points_list->DeleteAllItems();
    // Keep the reflow starting point
    this->points_list->InsertItem(0, wxT("0") );
    this->points_list->SetItem(0, 1, wxT("25"));
    // Refresh the graph
    this->update_graph();
}

/*
    Remove the selected point from the list
*/
void GUI_frame_ext::remove_point( wxCommandEvent& event )
{
    // Check that a line is selected from the points list
    if ( this->points_list->GetSelectedItemCount() == 0 ) {
        wxMessageBox( wxT("No line selected from the list"), wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // Get the index of the selected line
    int selected_item = this->points_list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    // Of course the default starting point added by default cannot be cancelled
    if ( selected_item == 0 ) {
        wxMessageBox( wxT("The first default point cannot be cancelled"), wxT("Error"), wxICON_INFORMATION);
        return;
    }

    // Remove the point from the list
    this->points_list->DeleteItem(selected_item);

    // Refresh the graph
    this->update_graph();
}

/**
 *  1) Send data points to the STM32
 *  2) Sends PIDs parameters to the STM32
 *  3) Starts the relow process
 */
void GUI_frame_ext::start( wxCommandEvent& event )
{
    // Clear the reflow list which might be loaded into the STM32
    STM32_device->clear_reflow_list();

    // Send data points to the STM32
    int num_items = this->points_list->GetItemCount();
    int index;
    wxListItem current_item;
    wxString time, temperature;

    for (index=0; index<num_items; index++){
        current_item.SetId(index);

        current_item.SetColumn(0);
        this->points_list->GetItem(current_item);
        time = current_item.GetText();
        current_item.SetColumn(1);
        this->points_list->GetItem(current_item);
        temperature = current_item.GetText();

        STM32_device->add_reflow_point(time, temperature);
    }

    // Send PIDs parameters
    wxString P_coeff, I_coeff, D_coeff;
    P_coeff = this->P_choice_1->GetString(this->P_choice_1->GetSelection());
    I_coeff = this->I_choice_1->GetString(this->I_choice_1->GetSelection());
    D_coeff = this->D_choice_1->GetString(this->D_choice_1->GetSelection());
    STM32_device->set_PID_parameters(1, P_coeff, I_coeff, D_coeff);

    P_coeff = this->P_choice_2->GetString(this->P_choice_2->GetSelection());
    I_coeff = this->I_choice_2->GetString(this->I_choice_2->GetSelection());
    D_coeff = this->D_choice_2->GetString(this->D_choice_2->GetSelection());
    STM32_device->set_PID_parameters(2, P_coeff, I_coeff, D_coeff);

    // Start the reflow process
    STM32_device->start_reflow_process();
}

/**
 *  Stops the relow process
 */
void GUI_frame_ext::stop( wxCommandEvent& event )
{
    STM32_device->stop_reflow_process();
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

