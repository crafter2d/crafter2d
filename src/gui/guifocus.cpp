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
#include "guiwnd.h"

#include "guifocus.h"
#ifndef JENGINE_INLINE
#  include "guifocus.inl"
#endif

#include "scopedvalue.h"

#include "guifocuslistener.h"

GuiFocus::GuiFocus():
   mListeners(),
   _focusWnd(),
   _changing(false)
{
}

GuiFocus& GuiFocus::getInstance()
{
   static GuiFocus focus;
   return focus;
}

void GuiFocus::setFocus(GuiWnd* pfocus)
{
   if ( !_changing )
   {
      ScopedValue<bool> value(_changing, true, false);

      GuiWnd* poldWnd = _focusWnd.instancePtr();

      if ( poldWnd != pfocus )
      {
         fireFocusChanged(*pfocus, poldWnd);

         _focusWnd = pfocus;
         /*
         if ( poldWnd != NULL )
            poldWnd->onKillFocus(pfocus);

         _focusWnd = pfocus;
         if ( _focusWnd.isAlive() )
            _focusWnd->onSetFocus(poldWnd);
         */
      }
   }
}

//-----------------------------------
// - Listeners
//-----------------------------------

void GuiFocus::addListener(GuiFocusListener& listener)
{
   mListeners.push_back(&listener);
}

void GuiFocus::removeListener(const GuiFocusListener& listener)
{
   Listeners::iterator it = std::find(mListeners.begin(), mListeners.end(), &listener);
   if ( it != mListeners.end() )
   {
      mListeners.erase(it);
   }
}

//-----------------------------------
// - Signaling
//-----------------------------------

void GuiFocus::fireFocusChanged(GuiWnd& newfocus, GuiWnd* poldfocus)
{
   for ( Listeners::size_type index = 0; index < mListeners.size(); ++index )
   {
      GuiFocusListener* plistener = mListeners[index];
      plistener->onFocusChanged(newfocus, poldfocus);
   }
}
