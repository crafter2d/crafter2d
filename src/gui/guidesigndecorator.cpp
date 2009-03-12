/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

#include "guidesignselection.h"

#include "guidesigndecorator.h"
#ifndef JENGINE_INLINE
#  include "guidesigndecorator.inl"
#endif

#include "../game.h"

#include "guidesigner.h"
#include "guidesignwnd.h"
#include "guiwindow.h"
#include "guimanager.h"

GuiDesignDecorator::GuiDesignDecorator():
   GuiControl(),
   MPControl(NULL),
   _selectionCtrl(NULL),
   MDragging(false)
{
}


GuiDesignDecorator::~GuiDesignDecorator()
{
}

//////////////////////////////////////////////////////////////////////////
// - Creation interface
//////////////////////////////////////////////////////////////////////////

void GuiDesignDecorator::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiControl::onCreate(rect, caption, style, parent);

   _selectionCtrl = new GuiDesignSelection();
   _selectionCtrl->create(0, GuiRect(), "", 0, this);
   _selectionCtrl->setVisible(false);
   
   setFont(parent->getFont());
   setParent(parent);
}

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

void GuiDesignDecorator::control(GuiControl* pcontrol)
{
   MPControl = pcontrol;
   
   const GuiRect& rect = control().getWindowRect();
   setWindowRect(rect);
   
   GuiRect newrect(0, rect.getWidth(), 0, rect.getHeight());
   control().setWindowRect(newrect);
   _selectionCtrl->setWindowRect(newrect);
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiDesignDecorator::onRender(Uint32 tick, const GuiGraphics& graphics)
{
   int i = 0;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

int GuiDesignDecorator::onLButtonDown (const GuiPoint& point, int flags)
{
   if ( _selectionCtrl->onLButtonDown(point, flags) == JENGINE_MSG_UNHANDLED )
   {
      MDragging = true;
   }

   bool hasSelection = selectionCtrl().getVisible();

   if ( !IS_SET(flags,GuiCtrl) )
   {
      dynamic_cast<GuiDesignWnd*>(getParent())->unselectAll();
      Game::getInstance().designer().focusChanged(control());

      hasSelection = false;
   }

   selectionCtrl().setVisible(!hasSelection);

   return JENGINE_MSG_HANDLED;
}

int GuiDesignDecorator::onLButtonUp (const GuiPoint& point, int flags)
{
   if ( MDragging )
   {
      MDragging = false;

      return JENGINE_MSG_HANDLED;
   }

   return _selectionCtrl->onLButtonUp(point, flags);
}

void GuiDesignDecorator::onKeyUp(int which)
{
   GuiDesignWnd* pwnd = dynamic_cast<GuiDesignWnd*>(getParent());
   
   switch ( which )
   {
   case SDLK_DELETE:
      pwnd->deleteSelected();
      break;
   }
}

bool GuiDesignDecorator::onContextMenu(const GuiPoint& point)
{
   GuiManager::getInstance().showPopup(*this, point, "des_control_layout_popup");
   return true;
}

void GuiDesignDecorator::onCommand(int cmd)
{
   GuiDesignWnd* pwnd = dynamic_cast<GuiDesignWnd*>(getParent());
   switch ( cmd )
   {
   case 0:  pwnd->align(*this, GuiDesignWnd::alignLeft);    break;
   case 1:  pwnd->align(*this, GuiDesignWnd::alignRight);   break;
   case 2:  pwnd->align(*this, GuiDesignWnd::alignTop);     break;
   case 3:  pwnd->align(*this, GuiDesignWnd::alignBottom);  break;

   case 4:  pwnd->sameSize(*this, GuiDesignWnd::sizeWidth);  break;
   case 5:  pwnd->sameSize(*this, GuiDesignWnd::sizeHeight); break;
   }
}

void GuiDesignDecorator::onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flags)
{
   if ( MDragging )
   {
      dynamic_cast<GuiDesignWnd*>(getParent())->moveSelected(rel);
   }
   else
   {
      _selectionCtrl->onMouseMove(point, rel, flags);
   }
}

void GuiDesignDecorator::onResize(int width, int height)
{
   control().resizeWindow(width, height);
   _selectionCtrl->resizeWindow(width, height);
}

GuiWnd* GuiDesignDecorator::hitTest(const GuiPoint &point)
{
   return m_frameRect.pointInRect(point) ? this : NULL;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiDesignDecorator::selected(bool select)
{
   _selectionCtrl->setVisible(select);
}
