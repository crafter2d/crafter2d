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
#ifndef GUITEXTBLOCK_H_
#define GUITEXTBLOCK_H_

#include <vector>
#include <string>

class GuiGraphics;
class GuiPoint;
class GuiWnd;

class GuiTextBlock
{
public:
   typedef std::vector<std::string> Strings;

                        GuiTextBlock(const GuiWnd* owner);
                        GuiTextBlock(const GuiWnd* owner, const std::string& str, bool linebreak);

   void                 paint(const GuiPoint& pos, const GuiGraphics& graphics) const;

   void                 assign(const std::string& line, bool linebreak);

   void                 owner(const GuiWnd* wnd);
   const GuiWnd*        owner() const;
   
   int                  height() const;

   const std::string&   text() const;

protected:
   int                  calculateMaxHorizontalChars();

private:
   int                  findLineBreakingSpace(const std::string& line, int pos, int len);

   Strings        _strings;
   const GuiWnd*  _owner;
   int            _height;
};

#endif
