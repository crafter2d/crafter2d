/***************************************************************************
 * Copyright (C) 2006 Jeroen Broekhuizen                                   *
 * jeroen@jeroen.homedns.org                                               *
 *                                                                         *
 * This library is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU Lesser General Public              *
 * License as published by the Free Software Foundation; either            * 
 * version 2.1 of the License, or (at your option) any later version.      *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library; if not, write to the                   *
 * Free Software Foundation, Inc.,                                         *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            *
 ***************************************************************************/
#ifndef GUIEVENT_HANDLER_DLG_H_
#define GUIEVENT_HANDLER_DLG_H_

#include "guidialog.h"

class GuiEventHandlerDlg : public GuiDialog
{
public:
   GuiEventHandlerDlg();

 // Get/set interface

   virtual void   sendMessage(GuiId id, GuiEvent event, int param1);

 // Operations
   void setCurrentWnd(GuiWnd* pwnd);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);

   void  onSelChanged(int sel);
   void  saveCode();

private:
   GuiWnd*     _pcurrentWnd;
   GuiListBox* _plist;
   GuiEditBox* _pedit;
   int         _currentEdit;
};

#ifdef JENGINE_INLINE
#  include "guieventhandlerdlg.inl"
#endif

#endif
