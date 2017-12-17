/***************************************************************
 * Name:      GUIApp.h
 * Purpose:   Defines Application Class
 * Author:    Valerio Setti ()
 * Created:   2016-04-30
 * Copyright: Valerio Setti ()
 * License:
 **************************************************************/

#ifndef GUIAPP_H
#define GUIAPP_H

#include <wx/app.h>
#include "GUI_interface_extend.h"
#include "libctb-0.16/include/ctb.h"

class MainClass : public wxApp
{
    public:
        virtual bool OnInit();

    private:
//        GUI_frame_ext* frame;
        GUI_frame_ext* frame;
        void find_COM_ports();
};

#endif // GUIAPP_H
