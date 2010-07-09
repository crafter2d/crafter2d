/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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

#include "guidesignwndmouselistener.h"

#include "gui/input/mouseevent.h"

#include "gui/guidesigner.h"
#include "gui/guidesignselection.h"
#include "gui/guidesignwnd.h"
#include "gui/guimenu.h"
#include "gui/guipopupdecorator.h"

GuiDesignWndMouseListener::GuiDesignWndMouseListener(GuiDesignWnd& window):
   MouseListener(),
   mWindow(window)
{
}

// - Notifications

void GuiDesignWndMouseListener::onMouseButton(const MouseEvent& event)
{
   if ( !event.isLeftButtonDown() )
   {
     return;
   }

   mWindow._pselectionctrl->getMouseListeners().fireMouseButtonEvent(event);
}

void GuiDesignWndMouseListener::onMouseContext(const MouseEvent& event)
{
   GuiDesigner* pdesigner = dynamic_cast<GuiDesigner*>(mWindow.getParent());
   ASSERT_PTR(pdesigner)
   
   GuiPopupDecorator* ppopup = mWindow._popupMenu;

   const Point& location = event.getLocation();

   if ( ppopup == NULL )
   {
            GuiMenu& menu = pdesigner->getWindowPopup();
      const GuiRect& rect = menu.getWindowRect();

      ppopup = new GuiPopupDecorator(menu);
      ppopup->create(100, GuiRect(location.x(), location.x() + rect.getWidth(), location.y(), location.y() + rect.getHeight()), "", GUI_VISIBLE, NULL);
      ppopup->setOwner(mWindow);

      mWindow._popupMenu = ppopup;
   }
   else
   {
      ppopup->setWindowPos(location.x(), location.y());
   }

   ppopup->popup();

   event.consume();
}

void GuiDesignWndMouseListener::onMouseMotion(const MouseEvent& event)
{
   if ( event.isLeftButtonDown() )
   {
      mWindow._pselectionctrl->getMouseListeners().fireMouseMotionEvent(event);
   }
}
