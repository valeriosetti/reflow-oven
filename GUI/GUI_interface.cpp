///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 30 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI_interface.h"

///////////////////////////////////////////////////////////////////////////

GUI_frame::GUI_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	top_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	refresh_list = new wxButton( this, wxID_ANY, wxT("Refresh List"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( refresh_list, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_choice1Choices;
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer2->Add( m_choice1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	connect = new wxButton( this, wxID_ANY, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( connect, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer9->Add( bSizer2, 0, wxEXPAND|wxBOTTOM, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Time [s]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer6->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString time_choiceChoices;
	time_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, time_choiceChoices, 0 );
	time_choice->SetSelection( 0 );
	bSizer6->Add( time_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Temperature [°C]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer6->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString temp_choiceChoices;
	temp_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, temp_choiceChoices, 0 );
	temp_choice->SetSelection( 0 );
	bSizer6->Add( temp_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	insert_point_btn = new wxButton( this, wxID_ANY, wxT("Insert Point"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( insert_point_btn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer6, 0, wxEXPAND|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Points list"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	points_list = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer3->Add( points_list, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	remove_point_btn = new wxButton( this, wxID_ANY, wxT("Remove Point"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( remove_point_btn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	clear_list_btn = new wxButton( this, wxID_ANY, wxT("Clear List"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( clear_list_btn, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	save_config_btn = new wxButton( this, wxID_ANY, wxT("Save config"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( save_config_btn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	reload_config_btn = new wxButton( this, wxID_ANY, wxT("Reload config"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( reload_config_btn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer3->Add( bSizer7, 0, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer3, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 2, 0, 0 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Product"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	gSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	wxArrayString P_choice_1Choices;
	P_choice_1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, P_choice_1Choices, 0 );
	P_choice_1->SetSelection( 0 );
	gSizer1->Add( P_choice_1, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Integral"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	gSizer1->Add( m_staticText6, 0, wxALL, 5 );
	
	wxArrayString I_choice_1Choices;
	I_choice_1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, I_choice_1Choices, 0 );
	I_choice_1->SetSelection( 0 );
	gSizer1->Add( I_choice_1, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Derivative"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer1->Add( m_staticText7, 0, wxALL, 5 );
	
	wxArrayString D_choice_1Choices;
	D_choice_1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, D_choice_1Choices, 0 );
	D_choice_1->SetSelection( 0 );
	gSizer1->Add( D_choice_1, 0, wxALL, 5 );
	
	bSizer8->Add( gSizer1, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxGridSizer* gSizer11;
	gSizer11 = new wxGridSizer( 3, 2, 0, 0 );
	
	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Product"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	gSizer11->Add( m_staticText51, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString P_choice_2Choices;
	P_choice_2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, P_choice_2Choices, 0 );
	P_choice_2->SetSelection( 0 );
	gSizer11->Add( P_choice_2, 0, wxALL, 5 );
	
	m_staticText61 = new wxStaticText( this, wxID_ANY, wxT("Integral"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	gSizer11->Add( m_staticText61, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString I_choice_2Choices;
	I_choice_2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, I_choice_2Choices, 0 );
	I_choice_2->SetSelection( 0 );
	gSizer11->Add( I_choice_2, 0, wxALL, 5 );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, wxT("Derivative"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	gSizer11->Add( m_staticText71, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString D_choice_2Choices;
	D_choice_2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, D_choice_2Choices, 0 );
	D_choice_2->SetSelection( 0 );
	gSizer11->Add( D_choice_2, 0, wxALL, 5 );
	
	bSizer8->Add( gSizer11, 1, wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer9->Add( bSizer8, 0, wxEXPAND, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer91->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Reflow process scan interval [ms]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer91->Add( m_staticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxArrayString scan_choiceChoices;
	scan_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, scan_choiceChoices, 0 );
	scan_choice->SetSelection( 0 );
	bSizer91->Add( scan_choice, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer91->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer91, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	stop_btn = new wxButton( this, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( stop_btn, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	start_btn = new wxButton( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( start_btn, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	bSizer9->Add( bSizer71, 0, wxEXPAND, 5 );
	
	top_sizer->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 50,-1 ), wxTAB_TRAVERSAL );
	graph_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_panel1->SetSizer( graph_sizer );
	m_panel1->Layout();
	bSizer11->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );
	
	save_graph_btn = new wxButton( this, wxID_ANY, wxT("Save graph"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( save_graph_btn, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	top_sizer->Add( bSizer11, 1, wxEXPAND, 5 );
	
	this->SetSizer( top_sizer );
	this->Layout();
	top_sizer->Fit( this );
	status_bar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	refresh_list->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::refresh_COM_list ), NULL, this );
	connect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::connect_to_COM ), NULL, this );
	insert_point_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::insert_point ), NULL, this );
	remove_point_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::remove_point ), NULL, this );
	clear_list_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::clear_list ), NULL, this );
	save_config_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::save_config ), NULL, this );
	reload_config_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::reload_config ), NULL, this );
	stop_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::stop ), NULL, this );
	start_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::start ), NULL, this );
	save_graph_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::save_graph ), NULL, this );
}

GUI_frame::~GUI_frame()
{
	// Disconnect Events
	refresh_list->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::refresh_COM_list ), NULL, this );
	connect->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::connect_to_COM ), NULL, this );
	insert_point_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::insert_point ), NULL, this );
	remove_point_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::remove_point ), NULL, this );
	clear_list_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::clear_list ), NULL, this );
	save_config_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::save_config ), NULL, this );
	reload_config_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::reload_config ), NULL, this );
	stop_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::stop ), NULL, this );
	start_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::start ), NULL, this );
	save_graph_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::save_graph ), NULL, this );
}
