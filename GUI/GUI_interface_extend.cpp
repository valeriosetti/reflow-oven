#include "GUI_interface_extend.h"
#include <wx/msgdlg.h>
#include "mathplot/mathplot.h"
#include <wx/filedlg.h>
#include <iostream>
#include <fstream>

// Local defines
#define MAX_NUM_POINTS      8

#define MAX_TEMPERATURE     300
#define MIN_TEMPERATURE     25
#define TEMP_INTERVAL       1

#define TIME_INTERVAL       5
#define MIN_TIME            0+TIME_INTERVAL
#define MAX_TIME            1200

#define COEFFS_INTERVAL     10.0
#define COEFFS_MIN_VALUE    0.0
#define COEFFS_MAX_VALUE    1000.0

#define MIN_SCAN            250
#define MAX_SCAN            2000
#define SCAN_INTERVAL       250

// Macros
#define TRY(_expr_)     if ((_expr_)!=0) {wxMessageBox( wxT("Communication error"), wxT("Error"), wxICON_INFORMATION);  \
                                            this->status_bar->SetStatusText(wxString("Disconnected"));                  \
                                            return;}

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

    // Set allowed scan interval values into the proper choicebox
    for (int scan_val=MIN_SCAN; scan_val<=MAX_SCAN; scan_val+=SCAN_INTERVAL) {
        this->scan_choice->Append( wxString::Format(wxT("%d"), scan_val) );
    }
    this->scan_choice->SetSelection(0);

    // Add the basic layers to the graph
    m_plot = new mpWindow( this, -1, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER );
	m_plot->AddLayer( new mpScaleX( wxT("Elapsed time [s]")));
    m_plot->AddLayer( new mpScaleY( wxT("Temperature [°C]")));
	//m_plot->EnableMousePanZoom(false);

	selected_point_layer = new mpFXYVector(wxT("selected_point_layer"));
	selected_point_layer->SetContinuity(true);
	wxPen vectorpen1(*wxRED, 1, wxSOLID);
	selected_point_layer->SetPen(vectorpen1);
	m_plot->AddLayer(selected_point_layer);

	thermocouple1_layer = new mpFXYVector(wxT("thermocouple1_layer"));
	thermocouple1_layer->SetContinuity(true);
	wxPen vectorpen2(*wxBLUE, 1, wxSOLID);
	thermocouple1_layer->SetPen(vectorpen2);
	m_plot->AddLayer(thermocouple1_layer);

	thermocouple2_layer = new mpFXYVector(wxT("thermocouple2_layer"));
	thermocouple2_layer->SetContinuity(true);
	wxPen vectorpen3(*wxGREEN, 1, wxSOLID);
	thermocouple2_layer->SetPen(vectorpen3);
    m_plot->AddLayer(thermocouple2_layer);

	top_sizer->Add( m_plot, 3, wxEXPAND);

	m_plot->SetMargins(100, 100, 100, 100);

	// Add the reflow starting point by default
    this->points_list->InsertItem(0, wxT("0") );
    this->points_list->SetItem(0, 1, wxT("25"));
    this->add_point_to_graph(selected_point_layer, 0.0, 25.0);

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
    this->add_point_to_graph(selected_point_layer, (float)time/1.0, (float)temperature/1.0);
}

/*
    Add a new point to the reflow list
*/
void GUI_frame_ext::clear_list( wxCommandEvent& event )
{
    points_list->DeleteAllItems();
    // Delete all the points from the plotting layers
    this->reset_graph(selected_point_layer);
    this->reset_graph(thermocouple1_layer);
    this->reset_graph(thermocouple2_layer);
    // Keep the reflow starting point
    this->points_list->InsertItem(0, wxT("0") );
    this->points_list->SetItem(0, 1, wxT("25"));
    this->add_point_to_graph(selected_point_layer, 0.0, 25.0);
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
    // TODO: remove the point also from the Layer and uèdate the plot
}

/**
 *  1) Send data points to the STM32
 *  2) Sends PIDs parameters to the STM32
 *  3) Starts the relow process
 */
void GUI_frame_ext::start( wxCommandEvent& event )
{
    // Clear thermocouples' plotting data
    this->reset_graph(thermocouple1_layer);
    this->reset_graph(thermocouple2_layer);

    // Clear the reflow list which might be loaded into the STM32
    TRY(STM32_device->clear_reflow_list());

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

        TRY(STM32_device->add_reflow_point(time, temperature));
    }

    // Send PIDs parameters
    wxString P_coeff, I_coeff, D_coeff;
    P_coeff = this->P_choice_1->GetString(this->P_choice_1->GetSelection());
    I_coeff = this->I_choice_1->GetString(this->I_choice_1->GetSelection());
    D_coeff = this->D_choice_1->GetString(this->D_choice_1->GetSelection());
    TRY(STM32_device->set_PID_parameters(1, P_coeff, I_coeff, D_coeff));

    P_coeff = this->P_choice_2->GetString(this->P_choice_2->GetSelection());
    I_coeff = this->I_choice_2->GetString(this->I_choice_2->GetSelection());
    D_coeff = this->D_choice_2->GetString(this->D_choice_2->GetSelection());
    TRY(STM32_device->set_PID_parameters(2, P_coeff, I_coeff, D_coeff));

    // Set the reflow process period
    wxString scan_interval;
    scan_interval = this->scan_choice->GetString(this->scan_choice->GetSelection());
    TRY(STM32_device->set_reflow_process_period(scan_interval));
    scan_interval.ToLong(&reflow_process_scan_interval);

    // Start the reflow process
    TRY(STM32_device->start_reflow_process());

    // Enable the timer for data reception
    this->Start(reflow_process_scan_interval, wxTIMER_ONE_SHOT);
}

/**
 *  This is the callback function for the wxTimer. It's used to get data from the STM32
 *  during the reflow process
 */
void GUI_frame_ext::Notify()
{
    uint32_t tick, target_temp, thermo1, thermo2;
    int ret_val = STM32_device->get_reflow_process_data(&tick, &target_temp, &thermo1, &thermo2);

    // In case of error notify the user
    if (ret_val < 0) {
        wxMessageBox( wxT("The reflow process will be stopped due to an error"), wxT("Error"), wxICON_INFORMATION);
        return;
    }
    // Notify also when the process terminates
    if (ret_val > 0) {
        wxMessageBox( wxT("The reflow process is completed"), wxT("Info"), wxICON_INFORMATION);
        return;
    }
    // Otherwise just plot the received data and re-start the timer
    // Note: the returned Tick value is in milliseconds whereas the plot is in seconds
    this->add_point_to_graph(thermocouple1_layer, (float)tick/1000.0, (float)thermo1/1.0);
    this->add_point_to_graph(thermocouple2_layer, (float)tick/1000.0, (float)thermo2/1.0);
    this->Start(reflow_process_scan_interval, wxTIMER_ONE_SHOT);
}

/**
 *  Stops the relow process
 */
void GUI_frame_ext::stop( wxCommandEvent& event )
{
    STM32_device->stop_reflow_process();
}

/**
 *  Add a point to the specified layer
 */
void GUI_frame_ext::add_point_to_graph(mpFXYVector* layer, float x, float y)
{
    layer->AddData(x,y);
    m_plot->FitXOnly(0.0f, MAX_TEMPERATURE);
}

/**
 *  Clear all the points from the specified layer
 */
void GUI_frame_ext::reset_graph(mpFXYVector* layer)
{
    layer->Clear();
    m_plot->FitXOnly(0.0f, MAX_TEMPERATURE);
}

/**
 *  Save the current configuration to a file
 */
void GUI_frame_ext::save_config( wxCommandEvent& event )
{
    wxFileDialog* OpenDialog = new wxFileDialog(
                        this, wxT("Choose the saving file"), wxEmptyString, wxT("reflow_config"),
                        wxT("Text files (*.txt)|*.txt"),
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK)
	{
        std::ofstream output_file;
        wxListItem temp_item;

        output_file.open(OpenDialog->GetPath(), std::ios::out);

        int item_count = this->points_list->GetItemCount();

        // Save the number of reflow points
        output_file << "reflow_point_num " << item_count << std::endl;

        // Save reflow points list
        for (int index=0; index < item_count; index++){
            temp_item.SetId(index);
            temp_item.SetColumn(0);
            this->points_list->GetItem(temp_item);
            output_file << "time " << temp_item.GetText() << " ";
            temp_item.SetColumn(1);
            this->points_list->GetItem(temp_item);
            output_file << "temp " << temp_item.GetText() << std::endl;
        }

        // Save PIDs parameters
        output_file << "P1_index " << this->P_choice_1->GetSelection() << std::endl;
        output_file << "I1_index " << this->I_choice_1->GetSelection() << std::endl;
        output_file << "D1_index " << this->D_choice_1->GetSelection() << std::endl;
        output_file << "P2_index " << this->P_choice_2->GetSelection() << std::endl;
        output_file << "I2_index " << this->I_choice_2->GetSelection() << std::endl;
        output_file << "D2_index " << this->D_choice_2->GetSelection() << std::endl;

        // Save the scan interval
        output_file << "scan_interval_index " << this->scan_choice->GetSelection() << std::endl;
	} else {
        return;
	}

	// Clean up after ourselves
	OpenDialog->Destroy();
}

/**
 *  Reload a previously saved configuration from a file
 */
void GUI_frame_ext::reload_config( wxCommandEvent& event )
{
   wxFileDialog* OpenDialog = new wxFileDialog(
                        this, wxT("Choose the config file"), wxEmptyString, wxT("reflow_config"),
                        wxT("Text files (*.txt)|*.txt"),
                        wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition);

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK)
	{
        std::ifstream input_file;
        int item_count, menu_index;
        std::string temp_string, time, temperature;

        input_file.open(OpenDialog->GetPath(), std::ios::in);

        // Reload reflow points
        input_file >> temp_string >> item_count;
        for (int index=0; index < item_count; index++) {
            input_file >> temp_string >> time >> temp_string >> temperature;
            this->points_list->InsertItem(index, time );
            this->points_list->SetItem(index, 1, temperature );
        }

        // Reload PIDs parameters
        input_file >> temp_string >> menu_index;
        this->P_choice_1->SetSelection(menu_index);
        input_file >> temp_string >> menu_index;
        this->I_choice_1->SetSelection(menu_index);
        input_file >> temp_string >> menu_index;
        this->D_choice_1->SetSelection(menu_index);
        input_file >> temp_string >> menu_index;
        this->P_choice_2->SetSelection(menu_index);
        input_file >> temp_string >> menu_index;
        this->I_choice_2->SetSelection(menu_index);
        input_file >> temp_string >> menu_index;
        this->D_choice_2->SetSelection(menu_index);

        // Reload the scan interval
        input_file >> temp_string >> menu_index;
        this->scan_choice->SetSelection(menu_index);

        //fclose(fp);
	} else {
        return;
	}

	// Clean up after ourselves
	OpenDialog->Destroy();
}

