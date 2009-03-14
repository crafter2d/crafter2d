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
#include "guicanvas.h"
#include "guipopupdecorator.h"
#ifndef JENGINE_INLINE
#  include "guipopupdecorator.inl"
#endif

#include "../game.h"

GuiPopupDecorator::GuiPopupDecorator():
   GuiWnd(),
   _ppopup(NULL),
   _powner(NULL),
   _owning(false)
{
}

GuiPopupDecorator::GuiPopupDecorator(GuiWnd& popup):
   GuiWnd(),
   _ppopup(&popup),
   _powner(NULL),
   _owning(false)
{
}

GuiPopupDecorator::~GuiPopupDecorator()
{
   if ( _ppopup != NULL )
   {
      _ppopup->setParent(NULL);
      _ppopup = NULL;
   }
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiPopupDecorator::sendMessage(GuiId id, GuiEvent event, int param1)
{
   if ( event == GuiLostFocusEvent )
      hide();

   if ( hasOwner() )
      getOwner().sendMessage(id, event, param1);
}

void GuiPopupDecorator::onCommand(int cmd)
{
   hide();

   if ( hasOwner() )
      getOwner().onCommand(cmd);
}

bool GuiPopupDecorator::isBoundaryWnd() const
{
   return true;
}

GuiWnd* GuiPopupDecorator::getBoundaryParent()
{
   return this;
}

void GuiPopupDecorator::popup()
{
   Game::getInstance().getCanvas().pushWindow(this);

   getPopup().setFocus();
}

void GuiPopupDecorator::hide()
{
   Game::getInstance().getCanvas().popWindow(this);

   if ( _owning )
   {
      removeChild(_ppopup);

      _ppopup->destroy();
      delete _ppopup;

      _ppopup = NULL;
   }
}
