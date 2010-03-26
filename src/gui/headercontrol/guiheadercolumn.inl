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

INLINE void GuiHeaderColumn::header(GuiHeaderCtrl* control)
{
   _header = control;
}

INLINE const GuiHeaderCtrl& GuiHeaderColumn::header() const
{
   return *_header;
}

INLINE GuiHeaderCtrl& GuiHeaderColumn::header()
{
   return const_cast<GuiHeaderCtrl&>(me().header());
}

INLINE void GuiHeaderColumn::editable(bool edit)
{
   _editable = edit;
}

INLINE bool GuiHeaderColumn::editable() const
{
   return _editable;
}

INLINE const std::string& GuiHeaderColumn::title() const
{
   return _text;
}

INLINE void GuiHeaderColumn::title(const std::string& text)
{
   _text = text;
}

INLINE int GuiHeaderColumn::width() const
{
   return _rect.getWidth();
}

INLINE GuiRect& GuiHeaderColumn::rect()
{
   return _rect;
}

INLINE const GuiHeaderColumn& GuiHeaderColumn::me()
{
   return static_cast<const GuiHeaderColumn&>(*this);
}
