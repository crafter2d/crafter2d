/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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

#include "guieditkeylistener.h"

#include "gui/input/keyevent.h"

#include "guiedit.h"

GuiEditBoxKeyListener::GuiEditBoxKeyListener(GuiEditBox& editbox):
   KeyListener(),
   mEditBox(editbox)
{
}

   //-----------------------------------
// - Handlers
//-----------------------------------

void GuiEditBoxKeyListener::onKeyPressed(const KeyEvent& event)
{
   switch ( event.getKey() )
   {
      case SDLK_BACKSPACE:
         mEditBox.doBackspace();
         break;
      case SDLK_DELETE:
         mEditBox.doDelete();
         break;
      case SDLK_HOME:
         mEditBox.moveHome();
         break;
      case SDLK_END:
         mEditBox.moveEnd();
         break;
      case SDLK_LEFT:
         mEditBox.moveLeft(event.isCtrlDown());
         break;
      case SDLK_RIGHT:
         mEditBox.moveRight(event.isCtrlDown());
         break;
      case SDLK_UP:
         mEditBox.moveUp();
         break;
      case SDLK_DOWN:
         mEditBox.moveDown();
         break;
      case SDLK_RETURN:
         mEditBox.doReturn(event.isShiftDown(), event.isCtrlDown(), event.isAltDown());
         break;
      case SDLK_TAB:
         mEditBox.doTab();
         break;
      default:
         if ( which >= 0 && which < 256 )
         {
            mEditBox.doInsert(which);
         }
         break;
   }

   mEditBox.ensureVisible();
}
