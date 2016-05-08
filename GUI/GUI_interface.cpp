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
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
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
	
	bSizer1->Add( bSizer2, 1, wxEXPAND|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Time [s]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer6->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	time_text = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	time_text->SetMaxLength( 4 ); 
	bSizer6->Add( time_text, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Temperature [°C]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer6->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	temp_text = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	temp_text->SetMaxLength( 4 ); 
	bSizer6->Add( temp_text, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	add_point_btn = new wxButton( this, wxID_ANY, wxT("Add Point"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( add_point_btn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer1->Add( bSizer6, 1, wxEXPAND|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Points list"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	points_list = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_NO_SORT_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer3->Add( points_list, 1, wxEXPAND, 5 );
	
	clear_list_btn = new wxButton( this, wxID_ANY, wxT("Clear List"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( clear_list_btn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer1->Add( bSizer3, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	start = new wxButton( this, wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( start, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	status_bar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	refresh_list->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::refresh_COM_list ), NULL, this );
	connect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::connect_to_COM ), NULL, this );
	add_point_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::add_point ), NULL, this );
	clear_list_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::clear_list ), NULL, this );
}

GUI_frame::~GUI_frame()
{
	// Disconnect Events
	refresh_list->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::refresh_COM_list ), NULL, this );
	connect->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::connect_to_COM ), NULL, this );
	add_point_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::add_point ), NULL, this );
	clear_list_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::clear_list ), NULL, this );
}
