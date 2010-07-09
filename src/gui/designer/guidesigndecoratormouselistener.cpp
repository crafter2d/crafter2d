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
#include "gui/guimanager.h"

#include "game.h"

GuiDesignDecoratorMouseListener::GuiDesignDecoratorMouseListener(GuiDesignDecorator& decorator):
   MouseListener(),
   mDecorator(decorator),
   mDragging(false)
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

   mDecorator.getSelectionCtrl().getMouseListeners().fireMouseButtonEvent(event);

   if ( event.getEventType() == MouseEvent::ePressed )
   {
      select(event.isCtrlDown());

      if ( !event.isConsumed() )
      {
        mDragging = true;
      }
   }
   else if ( event.getEventType() == MouseEvent::eReleased )
   {
      mDragging = false;
   }
}

void GuiDesignDecoratorMouseListener::onMouseContext(const MouseEvent& event)
{
   GuiManager::getInstance().showPopup(mDecorator, event.getLocation(), "des_control_layout_popup"); 

   event.consume();
}

void GuiDesignDecoratorMouseListener::onMouseMotion(const MouseEvent& event)
{
   if ( !event.isLeftButtonDown() )
   {
     return;
   }

   if ( mDragging )
   {
      dynamic_cast<GuiDesignWnd*>(mDecorator.getParent())->moveSelected(event.getRelative());
   }
   else
   {
      mDecorator.getSelectionCtrl().getMouseListeners().fireMouseMotionEvent(event);
   }
}

// - helpers
 
void GuiDesignDecoratorMouseListener::select(bool addSelection)
{
   bool selected = mDecorator.isSelected();

   if ( !addSelection )
   {
      dynamic_cast<GuiDesignWnd*>(mDecorator.getParent())->unselectAll();
      Game::getInstance().getCanvas().getDesigner().focusChanged(mDecorator.getControl());

      selected = false;
   }

   mDecorator.setSelected(!selected);
}
