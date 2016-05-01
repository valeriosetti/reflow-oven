///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 30 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_interface__
#define __GUI_interface__

#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GUI_frame
///////////////////////////////////////////////////////////////////////////////
class GUI_frame : public wxFrame 
{
	private:
	
	protected:
		wxChoice* m_choice1;
		wxButton* refresh_list;
		
		// Virtual event handlers, overide them in your derived class
		virtual void refresh_COM_list( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUI_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GUI_frame();
	
};

#endif //__GUI_interface__
