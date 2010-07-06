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
#include "guidesigndecoratormouselistener.h"

#include "gui/input/mouseevent.h"

#include "gui/guidesigner.h"
#include "gui/guidesigndecorator.h"
#include "gui/guidesignselection.h"
#include "gui/guidesignwnd.h"

#include "game.h"

GuiDesignDecoratorMouseListener::GuiDesignDecoratorMouseListener(GuiDesignDecorator& decorator):
   MouseListener(),
   mDecorator(decorator)
{
}

// - Notification

void GuiDesignDecoratorMouseListener::onMouseButton(const MouseEvent& event)
{
   if ( !event.isLeftButtonDown() )
   {
      // only respond on left button
      return;
   }

   mDecorator.mpSelectionCtrl->getMouseListeners().fireMouseButtonEvent(event);

   if ( event.getEventType() == MouseEvent::ePressed )
   {
      select(event.isCtrlDown());

      if ( !event.isConsumed() )
      {
        mDecorator.mDragging = true;
      }
   }
   else if ( event.getEventType() == MouseEvent::eReleased )
   {
      mDecorator.mDragging = false;
   }
}

// - helpers
 
void GuiDesignDecoratorMouseListener::select(bool addSelection)
{
   bool selected = mDecorator.isSelected();

   if ( !addSelection )
   {
      dynamic_cast<GuiDesignWnd*>(mDecorator.getParent())->unselectAll();
      Game::getInstance().getCanvas().getDesigner().focusChanged(mDecorator.control());

      selected = false;
   }

   mDecorator.setSelected(!selected);
}
