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
#include "../defines.h"

INLINE void GuiWnd::resizeWindow(int w, int h)
{
   m_frameRect.resize(w,h);
   onResize(w,h);
}

INLINE void GuiWnd::onResize(int width, int height)
{
}

INLINE GuiId GuiWnd::getId() const
{
   return m_id;
}

INLINE bool GuiWnd::hasParent() const
{
   return parent != NULL;
}

INLINE GuiWnd* GuiWnd::getParent() const
{
   return parent;
}

INLINE GuiStyle GuiWnd::getStyle() const
{
   return m_style;
}

INLINE const GuiRect& GuiWnd::getWindowRect() const
{
	return m_frameRect;
}

INLINE GuiFont* GuiWnd::getFont() const
{
	return font;
}

INLINE void GuiWnd::setFont (GuiFont* f)
{
	font = f;
}

INLINE void GuiWnd::setTopmost(bool topmost)
{
   if ( topmost )
      SET_FLAG(m_style, GUI_TOPMOST);
   else
      CLEAR_FLAG(m_style, GUI_TOPMOST);
}

INLINE bool GuiWnd::isTopmost() const
{
   return IS_SET(m_style, GUI_TOPMOST);
}

INLINE void GuiWnd::setWindowRect(const GuiRect& rect)
{
   setWindowPos(rect.left(), rect.top());
   resizeWindow(rect.getWidth(), rect.getHeight());
}

INLINE void GuiWnd::moveWindow (int x, int y)
{
	m_frameRect.offset (x, y);
}

INLINE GuiProperties& GuiWnd::getProperties()
{
   return MProperties;
}

INLINE GuiEventHandlerDefinitions& GuiWnd::getEventHandlerDefinitions()
{
   ASSERT_PTR(_peventhandlerdefinitions)
   return *_peventhandlerdefinitions;
}

INLINE GuiEventHandlers& GuiWnd::getEventHandlers()
{
   if ( _peventhandlers == NULL )
   {
      initializeEventHandlers();
   }

   ASSERT_PTR(_peventhandlers)
   return *_peventhandlers;
}
