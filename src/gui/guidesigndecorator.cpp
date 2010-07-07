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
#include "guidesigndecorator.h"
#ifndef JENGINE_INLINE
#  include "guidesigndecorator.inl"
#endif

#include "../game.h"

#include "input/mouseevent.h"

#include "guidesigner.h"
#include "guidesignwnd.h"
#include "guidesignselection.h"
#include "guiwindow.h"
#include "guimanager.h"

GuiDesignDecorator::GuiDesignDecorator():
   GuiControl(),
   mKeyListener(*this),
   mMouseListener(*this),
   mpSelectionCtrl(NULL),
   mpControl(NULL)
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

   addKeyListener(mKeyListener);
   addMouseListener(mMouseListener);

   mpSelectionCtrl = new GuiDesignSelection();
   mpSelectionCtrl->create(0, GuiRect(), "", 0, this);
   mpSelectionCtrl->setVisible(false);

   setFont(parent->getFont());
   setParent(parent);
}

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

void GuiDesignDecorator::setControl(GuiControl* pcontrol)
{
   mpControl = pcontrol;
   
   const GuiRect& rect = getControl().getWindowRect();
   setWindowRect(rect);
   
   GuiRect newrect(0, rect.getWidth(), 0, rect.getHeight());
   getControl().setWindowRect(newrect);
   getSelectionCtrl().setWindowRect(newrect);
}

INLINE bool GuiDesignDecorator::isSelected() const
{
   return getSelectionCtrl().getVisible();
}

INLINE void GuiDesignDecorator::setSelected(bool selected)
{
   getSelectionCtrl().setVisible(selected);
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiDesignDecorator::onRender(Uint32 tick, const GuiGraphics& graphics)
{
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

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

void GuiDesignDecorator::onResize(int width, int height)
{
   getControl().resizeWindow(width, height);
   getSelectionCtrl().resizeWindow(width, height);
}

GuiWnd* GuiDesignDecorator::hitTest(const GuiPoint &point)
{
   return m_frameRect.pointInRect(point) ? this : NULL;
}

void GuiDesignDecorator::resize(const GuiPoint& rel, int borders)
{
   getSelectionCtrl().resize(rel, borders);
}