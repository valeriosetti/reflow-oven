/***************************************************************
 * Name:      GUIApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Valerio Setti ()
 * Created:   2016-04-30
 * Copyright: Valerio Setti ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "main.h"

IMPLEMENT_APP(MainClass);

bool MainClass::OnInit()
{
//    this->frame = new GUI_frame_ext(0L);
    this->frame = new GUI_frame_ext(0L);

    frame->Show();

    return true;
}

