///////////////////////////////////////////////////////////////////////////
#ifndef GUI_INTERFACE_EXTEND_H_INCLUDED
#define GUI_INTERFACE_EXTEND_H_INCLUDED

#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/string.h>

///////////////////////////////////////////////////////////////////////////
#include "GUI_interface.h"
#include "ctb-0.16/ctb.h"

class GUI_frame_ext : public GUI_frame
{
	private:

	protected:

	public:
        GUI_frame_ext( wxWindow* parent );
        ~GUI_frame_ext();
        void refresh_COM_list( wxCommandEvent& event );
};

#endif // GUI_INTERFACE_EXTEND_H_INCLUDED
