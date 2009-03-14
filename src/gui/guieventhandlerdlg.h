/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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
