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
#ifndef GUI_CANVAS_WND_OBSERVER_H_
#define GUI_CANVAS_WND_OBSERVER_H_

#include "guiwndobserver.h"

class GuiWndReference : public GuiWndObserver
{
public:
            GuiWndReference();
   explicit GuiWndReference(GuiWnd& wnd);
   explicit GuiWndReference(const GuiWndReference& reference);
   virtual  ~GuiWndReference();

   GuiWndReference& operator=(const GuiWndReference& reference);
   GuiWndReference& operator=(GuiWnd* pwnd);

 // querying
   bool isAlive() const;

   GuiWnd& instance();
   GuiWnd* instancePtr();

 // operators
   GuiWnd& operator* () const;
   GuiWnd* operator-> () const;

   bool operator==(const GuiWndReference& reference) const;
   bool operator==(const GuiWnd& wnd) const;

   void clear();

 // notifications
   virtual void notifyDestroyed(GuiWnd& wnd);

private:
   GuiWnd* _pwnd; // not owner
};

#ifdef JENGINE_INLINE
#  include "guiwndreference.inl"
#endif

#endif
