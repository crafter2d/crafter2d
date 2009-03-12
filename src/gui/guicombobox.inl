/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "../defines.h"

INLINE void GuiComboBox::setCallback(const char* fnc)
{
   callbackFnc = fnc;
}

INLINE void GuiComboBox::setItemText(int index, const char* text)
{
   options->setItemText(index, text);
}

INLINE void GuiComboBox::setItemData(int index, Uint32 data)
{
   options->setItemData(index, data);
}

INLINE void GuiComboBox::setCurSel(int index)
{
   options->setCurSel(index);
}

INLINE const char* GuiComboBox::getItemText(int index) const
{
   return options->getItemText(index);
}

INLINE Uint32 GuiComboBox::getItemData(int index) const
{
   return options->getItemData(index);
}

INLINE int GuiComboBox::getCurSel() const
{
   return options->getCurSel();
}

INLINE void GuiColorComboBox::addColor(GuiColor color)
{
   ((GuiColorListBox*)options)->addColor(color);
}

INLINE const GuiColor& GuiColorComboBox::GuiColorListBox::getColor(int idx) const
{
   return colors[idx];
}
