/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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

#include "guicolorcombobox.h"

#include "gui/colorlistbox/guicolorlistbox.h"
#include "gui/guigraphics.h"
#include "gui/guipopupdecorator.h"

GuiColorComboBox::GuiColorComboBox():
   GuiComboBox()
{
}

void GuiColorComboBox::addColor(GuiColor color)
{
   ((GuiColorListBox*)options)->addColor(color);
}

void GuiColorComboBox::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiComboBox::onCreate(rect, caption, style, parent);

   // remove the current options box from the window
   GuiRect client(options->getWindowRect());
   delete _list;

   // create our own options listbox
   options = new GuiColorListBox();

   _list = new GuiPopupDecorator(*options);
   _list->create(3, client, "", GUI_VISIBLE, NULL);
   _list->setOwner(*this);

   options->create(1, client, "", GUI_BACKGROUND|GUI_BORDER|GUI_VISIBLE|GUI_NOTIFYPARENT, _list);
}

void GuiColorComboBox::paint(Uint32 tick, const GuiGraphics& graphics)
{
   GuiComboBox::paint(tick, graphics);

   float pLX = m_frameRect.left() + 5;
   float pUY = m_frameRect.top() + 5;
   float pRX = pLX + m_frameRect.getWidth() - 44;
   float pBY = pUY + m_frameRect.getHeight() - 10;

   GuiColorListBox* clb = static_cast<GuiColorListBox*>(options);

   graphics.setColor(clb->getColor(clb->getCurSel()));
   graphics.drawRect(GuiRect(pLX, pRX, pUY, pBY));
}
