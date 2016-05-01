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
	
	wxString m_choice1Choices[] = { wxT("aaa"), wxT("bbb"), wxT("ccc") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer2->Add( m_choice1, 1, wxALL, 5 );
	
	refresh_list = new wxButton( this, wxID_ANY, wxT("Refresh List"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( refresh_list, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	refresh_list->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::refresh_COM_list ), NULL, this );
}

GUI_frame::~GUI_frame()
{
	// Disconnect Events
	refresh_list->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUI_frame::refresh_COM_list ), NULL, this );
}
