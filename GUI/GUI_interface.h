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
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
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
		wxStaticLine* m_staticline1;
		
		wxStaticText* m_staticText3;
		wxChoice* time_choice;
		wxStaticText* m_staticText4;
		wxChoice* temp_choice;
		wxButton* insert_point_btn;
		
		wxStaticText* m_staticText2;
		wxListCtrl* points_list;
		wxButton* remove_point_btn;
		wxButton* clear_list_btn;
		
		wxButton* save_config_btn;
		wxButton* reload_config_btn;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText5;
		wxChoice* P_choice_1;
		wxStaticText* m_staticText6;
		wxChoice* I_choice_1;
		wxStaticText* m_staticText7;
		wxChoice* D_choice_1;
		wxStaticText* m_staticText51;
		wxChoice* P_choice_2;
		wxStaticText* m_staticText61;
		wxChoice* I_choice_2;
		wxStaticText* m_staticText71;
		wxChoice* D_choice_2;
		wxStaticLine* m_staticline3;
		
		wxStaticText* m_staticText10;
		wxChoice* scan_choice;
		
		wxStaticLine* m_staticline4;
		wxButton* stop_btn;
		wxButton* start_btn;
		wxPanel* m_panel1;
		wxBoxSizer* graph_sizer;
		wxButton* save_graph_btn;
		wxStatusBar* status_bar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void refresh_COM_list( wxCommandEvent& event ) { event.Skip(); }
		virtual void connect_to_COM( wxCommandEvent& event ) { event.Skip(); }
		virtual void insert_point( wxCommandEvent& event ) { event.Skip(); }
		virtual void remove_point( wxCommandEvent& event ) { event.Skip(); }
		virtual void clear_list( wxCommandEvent& event ) { event.Skip(); }
		virtual void save_config( wxCommandEvent& event ) { event.Skip(); }
		virtual void reload_config( wxCommandEvent& event ) { event.Skip(); }
		virtual void stop( wxCommandEvent& event ) { event.Skip(); }
		virtual void start( wxCommandEvent& event ) { event.Skip(); }
		virtual void save_graph( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUI_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GUI_frame();
	
};

#endif //__GUI_interface__
