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
#ifndef GUI_HEADER_COLUMN_H_
#define GUI_HEADER_COLUMN_H_

#include <string>
#include "guihelpers.h"

class GuiHeaderCtrl;
class GuiGraphics;

class GuiHeaderColumn
{
public:
                           GuiHeaderColumn(GuiHeaderCtrl* header, int pixels);
                           GuiHeaderColumn(GuiHeaderCtrl* header, int left, int pixels);

   void                    header(GuiHeaderCtrl* control);
   const GuiHeaderCtrl&    header() const;
   GuiHeaderCtrl&          header();

   const std::string&      title() const;
   void                    title(const std::string& text);

   void                    editable(bool edit);
   bool                    editable() const;

 // operations
   void                    paint(const GuiGraphics& graphics);
   void                    resize(int relx);

   int                     width() const;

   GuiRect&                rect();

protected:
   const GuiHeaderColumn&  me();

private:
   GuiHeaderCtrl* _header;
   std::string    _text;
   GuiRect        _rect;
   int            _width;
   bool           _editable;
};

#ifdef JENGINE_INLINE
#  include "guiheadercolumn.inl"
#endif

#endif
