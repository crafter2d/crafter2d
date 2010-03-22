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
#include "defines.h"

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

/// \fn GuiCanvas::isActiveWindow(GuiWindow* window) const
/// \brief Tests if the given window is the currently active one.
INLINE bool GuiCanvas::isActiveWnd(const GuiWnd& window) const
{
   return activeWnd == window;
}

/// \fn GuiCanvas::getActiveWnd()
/// \brief Returns the control with the focus of the active window
INLINE GuiWnd& GuiCanvas::getActiveWnd() const
{
   return *activeWnd;
}

/// \fn GuiCanvas::changeDefaultColor(GuiStandardColor color, const GuiColor& newColor)
/// \brief Sets the new default color.
INLINE void GuiCanvas::changeDefaultColor(GuiStandardColor color, const GuiColor& newColor)
{
   defaultColors[color] = newColor;
}

INLINE void GuiCanvas::changeDefaultColor(int color, const GuiColor& newColor)
{
   changeDefaultColor(static_cast<GuiStandardColor>(color), newColor);
}

INLINE GuiColor GuiCanvas::getDefaultColor(GuiStandardColor color) const
{
   return defaultColors[color];
}

INLINE bool GuiCanvas::isDesigning() const
{
   return mpDesigner != NULL;
}

INLINE GuiDesigner& GuiCanvas::getDesigner()
{
   ASSERT_PTR(mpDesigner);
   return *mpDesigner;
}