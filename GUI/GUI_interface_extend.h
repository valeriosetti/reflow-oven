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
#include <wx/timer.h>

///////////////////////////////////////////////////////////////////////////
#include "GUI_interface.h"
#include "communication.h"
#include "libctb-0.16/include/portscan.h"
#include "libctb-0.16/include/ctb.h"
#include "mathplot/mathplot.h"
#include "stdint.h"
#include <string.h>
#include <sstream>
#include <wx/msgdlg.h>

class GUI_frame_ext : public GUI_frame, wxTimer
{
	private:
        Communication* STM32_device;
        long reflow_process_scan_interval;
        mpWindow* m_plot;
        mpFXYVector* selected_point_layer;
        mpFXYVector* thermocouple1_layer;
        mpFXYVector* thermocouple2_layer;

        void add_point_to_graph(mpFXYVector* layer, float x, float y);
        // void remove_point_to_graph(mpFXYVector* layer, float x, float y); // TODO
        void reset_graph(mpFXYVector* layer);
        void ChildNotifiesConnection(bool status);
        void Notify();

	protected:

	public:
        GUI_frame_ext( wxWindow* parent );
        ~GUI_frame_ext();
        void refresh_COM_list( wxCommandEvent& event );
        void connect_to_COM( wxCommandEvent& event );
        void insert_point( wxCommandEvent& event );
        void clear_list( wxCommandEvent& event );
        void remove_point( wxCommandEvent& event );
        void stop( wxCommandEvent& event );
		void start( wxCommandEvent& event );
		void save_config( wxCommandEvent& event );
		void reload_config( wxCommandEvent& event );
		void save_graph( wxCommandEvent& event );
};

#endif // GUI_INTERFACE_EXTEND_H_INCLUDED
