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
#include "guistatic.h"

#include "guimanager.h"
#include "guidesigner.h"
#include "guifont.h"
#include "guitext.h"

REGISTER_DESIGNER(GuiStatic, GuiLabelId, "Label", 30, 16, 8)

void GuiStatic::initializeProperties()
{
   GuiWnd::initializeProperties();

   //MPCaption->registerCallback(onCaptionChanged);
   getCaptionProperty().registerCallback(onCaptionChanged);
}

void GuiStatic::onCaptionChanged(GuiWnd* pwnd)
{
   GuiStatic* pstatic = dynamic_cast<GuiStatic*>(pwnd);
   pstatic->captionChanged();
}

GuiStatic::GuiStatic():
   maxLines(-1),
   maxChars(0)
{
}

GuiStatic::GuiStatic(int id, const GuiRect& rect, const char* caption, int style, GuiWnd* parent):
   maxLines(-1),
   maxChars(0)
{
   //create(id, rect, caption, style, parent);
}

void GuiStatic::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
	GuiControl::onCreate(rect, caption, style, parent);

   captionChanged();
}

void GuiStatic::captionChanged()
{
   int fontHeight = font->getHeight();
   const std::string& text = getCaption(); //MPCaption->Value();
   int len = text.size();
   int pos = 0, idx = -1;

   // remove all the previous strings
   strings.clear();

   if (maxLines == -1)
   {
      maxLines = m_frameRect.getHeight() / font->getHeight();
      maxChars = m_frameRect.getWidth() / font->getAverageWidth();
   }

   while (pos < len) {
      // determine the number of chars to render
      int templen = maxChars;
      if (pos + templen >= len)
         templen = len - pos;
      else
      {
         templen = getSpacePosition(text.c_str(), pos, templen);
         if (templen <= 0)
         {
            templen = maxChars;
         }
         else if (text[pos+templen] == ' ')
            templen++;
      }

      // see if there is an eol character
      std::string line = text.substr(pos, templen);
      int location = line.find('\n');
      if (location != -1)
      {
         if (location > 0 && line[location-1] == '\r')
            location--;
         line = line.substr(0, location);
         templen = location+2;
      }

      // add the string to the list
      strings.push_back(line);
      pos += templen;
   }
}

void GuiStatic::onResize(int width, int height)
{
   maxLines = -1;

   captionChanged();
}

int GuiStatic::getSpacePosition(const char* text, int pos, int len)
{
   int s = pos+len;
   for (; s > pos; --s) {
      if (text[s] == ' ')
         break;
   }
   return s - pos;
}

void GuiStatic::paint(Uint32 tick, const GuiGraphics& graphics)
{
   graphics.setColor(GuiManager::getInstance().getDefaultTextColor());

   int height = font->getBaseHeight();
   int y = m_frameRect.top()+height;
   for (int i = 0; i < strings.size(); ++i, y += height)
   {
      GuiText::printf(*font, m_frameRect.left(), y, strings[i].c_str());
   }
}
