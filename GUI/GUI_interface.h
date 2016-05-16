///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 30 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_interface__
#define __GUI_interface__

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/listctrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GUI_frame
///////////////////////////////////////////////////////////////////////////////
class GUI_frame : public wxFrame 
{
	private:
	
	protected:
		wxBoxSizer* top_sizer;
		wxButton* refresh_list;
		wxChoice* m_choice1;
		wxButton* connect;
		wxStaticText* m_staticText3;
		wxTextCtrl* time_text;
		wxStaticText* m_staticText4;
		wxTextCtrl* temp_text;
		wxButton* insert_point_btn;
		wxStaticText* m_staticText2;
		wxListCtrl* points_list;
		wxButton* remove_point_btn;
		wxButton* clear_list_btn;
		wxButton* start;
		wxStatusBar* status_bar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void refresh_COM_list( wxCommandEvent& event ) { event.Skip(); }
		virtual void connect_to_COM( wxCommandEvent& event ) { event.Skip(); }
		virtual void insert_point( wxCommandEvent& event ) { event.Skip(); }
		virtual void remove_point( wxCommandEvent& event ) { event.Skip(); }
		virtual void clear_list( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUI_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 882,442 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GUI_frame();
	
};

#endif //__GUI_interface__
