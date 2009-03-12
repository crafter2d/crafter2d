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
#ifndef GUILISTBOXSUBITEM_H_
#define GUILISTBOXSUBITEM_H_

#include <string>
#include "SDL/SDL.h"
#include "guihelpers.h" 

class GuiGraphics;
class GuiListBoxItem;
class GuiTextBlock;

class GuiListBoxSubItem
{
public:
                           GuiListBoxSubItem();
   explicit                GuiListBoxSubItem(const GuiListBoxItem& i);
                           GuiListBoxSubItem(const GuiListBoxItem& i, const std::string& str);
   virtual                 ~GuiListBoxSubItem();

   void                    text(const std::string& t);
   const std::string&      text() const;

   const GuiListBoxItem&   item() const;

   bool                    hasBlock() const;
   const GuiTextBlock&     block() const;

   void                    paint(int column, const GuiPoint& pos, const GuiGraphics& graphics);

private:
   void                    destroy();

   GuiRect                 calculateVisibleRect(int column, const GuiPoint& pos);

   const GuiListBoxItem&   _item;
   GuiTextBlock*           _pblock;
   std::string             _text;
};

#ifdef JENGINE_INLINE
#  include "guilistboxsubitem.inl"
#endif

#endif
