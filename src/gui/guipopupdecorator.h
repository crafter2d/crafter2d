/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef GUI_POPUP_DECORATOR_H_
#define GUI_POPUP_DECORATOR_H_

#include "guiwnd.h"

class GuiPopupDecorator : public GuiWnd
{
public:
            GuiPopupDecorator();
   explicit GuiPopupDecorator(GuiWnd& popup);
            ~GuiPopupDecorator();

   bool     hasPopup() const;
   GuiWnd&  getPopup();
   void     setPopup(GuiWnd& popup, bool owning = false);

   bool     hasOwner() const;
   GuiWnd&  getOwner();
   void     setOwner(GuiWnd& owner);

 // Operations
   void              popup();
   void              hide();

   virtual void      sendMessage(GuiId id, GuiEvent event, int param1=0);
   virtual void      onCommand(int cmd);

   virtual bool      isBoundaryWnd() const;
   virtual GuiWnd*   getBoundaryParent();

private:
   GuiWnd* _ppopup;
   GuiWnd* _powner;
   bool    _owning;
};

#ifdef JENGINE_INLINE
#  include "guipopupdecorator.inl"
#endif

#endif
