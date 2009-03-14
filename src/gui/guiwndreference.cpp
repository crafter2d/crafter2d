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
#include "guiwndreference.h"
#ifndef JENGINE_INLINE
#  include "guiwndreference.inl"
#endif

#include "guiwnd.h"

GuiWndReference::GuiWndReference():
   _pwnd(NULL)
{
}

GuiWndReference::GuiWndReference(GuiWnd& wnd):
   _pwnd(&wnd)
{
   if ( isAlive() )
      _pwnd->attach(*this);
}

GuiWndReference::GuiWndReference(const GuiWndReference& reference):
   _pwnd(reference._pwnd)
{
   if ( isAlive() )
      _pwnd->attach(*this);
}

GuiWndReference::~GuiWndReference()
{
}

GuiWndReference& GuiWndReference::operator=(const GuiWndReference& reference)
{
   return operator=(reference._pwnd);
}

GuiWndReference& GuiWndReference::operator=(GuiWnd* pwnd)
{
   if ( isAlive() )
      _pwnd->detach(*this);

   _pwnd = pwnd;

   if ( isAlive() )
      _pwnd->attach(*this);

   return *this;
}

//////////////////////////////////////////////////////////////////////////
// - Notifications
//////////////////////////////////////////////////////////////////////////

void GuiWndReference::notifyDestroyed(GuiWnd& wnd)
{
   if ( &wnd == _pwnd )
   {
      _pwnd = NULL;
   }
}
