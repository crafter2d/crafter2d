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
#include "guimenu.h"
#ifndef JENGINE_INLINE
#  include "guimenu.inl"
#endif

#include "core/defines.h"
#include "../script.h"
#include "../scriptmanager.h"
#include "../scopedvalue.h"

#include "guifont.h"
#include "guieventhandler.h"
#include "guieventhandlers.h"
#include "guieventhandlerdefinition.h"
#include "guieventhandlerdefinitions.h"
#include "guitext.h"
#include "guifocus.h"

GuiMenu::GuiMenu():
   GuiControl(),
   GuiFocusListener(),
   MItems(),
   _name(),
   mMouseListener(*this),
   _popupLocation(),
   _selection(-1),
   _processing(false)
{
   GuiFocus::getInstance().addListener(*this);
}

GuiMenu::~GuiMenu()
{
   GuiFocus::getInstance().removeListener(*this);
}

// - Initialization

void GuiMenu::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   CLEAR_FLAG(style, GUI_NOTIFYPARENT);

   GuiWnd::onCreate(rect, caption, style, parent);

   addMouseListener(mMouseListener);
}

void GuiMenu::initializeEventHandlerDefinitions()
{
   GuiControl::initializeEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiContextMenuEvent, "onClick");
   pdefinition->addArgument("id");
   getEventHandlerDefinitions().add(pdefinition);
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiMenu::onRender(Uint32 tick, const GuiGraphics& graphics)
{
   bool enabled = glIsEnabled(GL_SCISSOR_TEST);
   glDisable(GL_SCISSOR_TEST);

   graphics.drawRaisedRect(getWindowRect());

   int y = getWindowRect().top() + getFont()->getBaseHeight() + 5;
   for ( int idx = 0; idx < MItems.size(); ++idx )
   {
      GuiMenuItem& item = MItems[idx];
      if ( idx == _selection )
      {
         graphics.setColor(0.5f,0.5f,0.5f);
         const GuiRect& rect = getWindowRect();
         int dy = y-getFont()->getBaseHeight();
         graphics.drawRect(GuiRect(rect.left()+3, rect.right()-3, dy, dy+getFont()->getHeight()));

         graphics.setColor(1,1,1);
      }
      else
         graphics.setColor(0,0,0);

      GuiText::printf(*getFont(), getWindowRect().left() + 5, y, item.text().c_str());
      y += getFont()->getHeight();
   }

   if ( enabled )
      glEnable(GL_SCISSOR_TEST);
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiMenu::addItem(int id, const std::string& item)
{
   MItems.push_back(GuiMenuItem(id, item));
}

void GuiMenu::calculateSize()
{
   int height = getFont()->getHeight() * MItems.size() + 10;
   const int avwidth = getFont()->getAverageWidth();
   int width = 0;

   for ( int index = 0; index < MItems.size(); ++index )
   {
      const GuiMenuItem& item = MItems[index];
      int itemwidth = item.text().length() * avwidth;
      if ( itemwidth > width)
         width = itemwidth;
   }

   resizeWindow(width + 10, height);
}

void GuiMenu::selectItem(const GuiPoint& point)
{
   GuiRect rect(0, getWindowRect().getWidth(), 0, getWindowRect().getHeight());

   if ( rect.pointInRect(point) )
   {
      _selection = (point.y - rect.top() - 5) / getFont()->getHeight();
      if ( _selection < 0 || _selection >= MItems.size() )
         _selection = -1;
   }
   else
      _selection = -1;
}

// notifications

void GuiMenu::onFocusChanged(GuiWnd& newFocus, GuiWnd* poldFocus)
{
   GuiControl::onKillFocus(&newFocus);

   if ( !_processing && poldFocus == this )
   {
      getParent()->sendMessage(getId(), GuiLostFocusEvent);
   }
}
