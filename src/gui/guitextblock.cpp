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

#include "guitextblock.h"
#ifndef JENGINE_INLINE
#  include "guitextblock.inl"
#endif

#include "guifont.h"
#include "guihelpers.h"
#include "guigraphics.h"
#include "guiwnd.h"
#include "guitext.h"

GuiTextBlock::GuiTextBlock(const GuiWnd* parent):
   _owner(parent),
   _height(0)
{
}

GuiTextBlock::GuiTextBlock(const GuiWnd* parent, const std::string& str, bool linebreak):
   _owner(parent),
   _height(0)
{
   assign(str, linebreak);
}

void GuiTextBlock::assign(const std::string& line, bool linebreak)
{
   const int fontHeight = owner()->getFont()->getHeight();
   const int maxChars = calculateMaxHorizontalChars();
   const int len = line.length();
   int pos = 0;

   if ( !linebreak || len < maxChars )
   {
      _strings.push_back(line);
      _height = fontHeight;
   }
   else
   {
      while (pos < len)
      {
         int templen = maxChars;
         if (pos + templen >= len)
            templen = len - pos;
         else
         {
            templen = findLineBreakingSpace(line.c_str(), pos, templen);
            if (templen == -1)
            {
               templen = maxChars;
            }
         }

         _strings.push_back(line.substr(pos, templen));
         _height += fontHeight;

         pos += templen;
      }
   }
}

int GuiTextBlock::calculateMaxHorizontalChars()
{
   int charWidth = owner()->getFont()->getAverageWidth();
   const GuiRect& rect = owner()->getWindowRect();
   return ( (rect.getWidth() - 16) / charWidth );
}

int GuiTextBlock::findLineBreakingSpace(const std::string& line, int pos, int len)
{
   for ( int x = pos + len; x >= pos; --x )
   {
      if ( line[x] == ' ' )
         return x;
   }
   return -1;
}

void GuiTextBlock::paint(const GuiPoint& pos, const GuiGraphics& graphics) const
{
   int y = pos.y;
   Strings::const_iterator it = _strings.begin();
   const int fontHeight = owner()->getFont()->getHeight();
   for ( ; it != _strings.end(); ++it, y += fontHeight)
   {
      const std::string& str = (*it);
      GuiText::printfn(*owner()->getFont(), pos.x+5, y+2+owner()->getFont()->getBaseHeight(), str.c_str(), str.size());
   }
}
