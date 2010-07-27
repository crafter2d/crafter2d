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

#include "guidesignermouselistener.h"

#include "gui/input/mouseevent.h"

#include "gui/guidesigner.h"
#include "gui/guipopupdecorator.h"
#include "gui/guimenu.h"

GuiDesignerMouseListener::GuiDesignerMouseListener(GuiDesigner& designer):
   MouseListener(),
   mDesigner(designer)
{
}

// - Notifications

void GuiDesignerMouseListener::onMouseContext(const MouseEvent& event)
{
   const GuiRect& rect = mDesigner.getDesignerPopup().getWindowRect();

   GuiPopupDecorator* ppopup = mDesigner._popupMenu;

   const Point& location = event.getLocation();

   if ( ppopup == NULL )
   {
      ppopup = new GuiPopupDecorator(mDesigner.getDesignerPopup());
      ppopup->create(100, GuiRect(location.x(), location.x() + rect.getWidth(), location.y(), location.y() + rect.getHeight()), "", GUI_VISIBLE, NULL);
      ppopup->setOwner(mDesigner);

      mDesigner._popupMenu = ppopup;

      mDesigner.getDesignerPopup().setParent(ppopup);
   }
   else
   {
      ppopup->setWindowPos(location.x(), location.y());
   }

   ppopup->popup();
}