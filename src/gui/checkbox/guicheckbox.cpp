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
#include "guicheckbox.h"

#include "gui/guidesigner.h"
#include "gui/guifont.h"
#include "gui/guimanager.h"
#include "gui/guitext.h"

#include "core/defines.h"

REGISTER_DESIGNER(GuiCheckBox, GuiCheckBoxId, "Checkbox", 40, 15, 264)

GuiCheckBox::GuiCheckBox():
   GuiControl(),
   mMouseListener(*this),
   pos(),
   box()
{
}

void GuiCheckBox::onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiControl::onCreate(rect, caption, style, parent);

   addMouseListener(mMouseListener);
}

bool GuiCheckBox::isChecked() const
{
   return IS_SET(m_style, GUI_PRESSED);
}

void GuiCheckBox::swapChecked()
{
   SWAP_FLAG(m_style, GUI_PRESSED);
}

void GuiCheckBox::paint(Uint32 tick, const GuiGraphics& graphics)
{
   graphics.drawSunkenRect(box);

   if ( isChecked() )
   {
      // draw the cross
      glLineWidth(1.5f);
      glBegin(GL_LINES);
         glColor3f(0,0,0);
         glVertex2i(box.left()+3, box.top()+3);
         glVertex2i(box.right()-3, box.bottom()-3);
         glVertex2i(box.left()+3, box.bottom()-3);
         glVertex2i(box.right()-3, box.top()+3);
         glColor3f(1,1,1);
      glEnd();
      glLineWidth(1.0f);
   }

   // draw the text
   if ( !getCaption().empty() )
   {
      GuiFont* font = parent->getFont ();
      GuiColor color(GuiManager::getInstance().getDefaultTextColor());
      glColor3fv(&color.r);
      GuiText::printfn(*font, pos.x, pos.y, getCaption());
   }
}

void GuiCheckBox::onResize(int width, int height)
{
   int halfHeight = height / 2;

   box.left(getWindowRect().left());
   box.right(getWindowRect().left() + 12);
   box.top(getWindowRect().top() + (halfHeight - 6));
   box.bottom(getWindowRect().top() + 12);

   pos.x = box.right() + 5;
   pos.y = box.top() + getFont()->getBaseHeight() - 2;
}
