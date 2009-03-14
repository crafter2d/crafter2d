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
#ifndef GUI_TREEBOX_ITEM_H_
#define GUI_TREEBOX_ITEM_H_

#include <string>
#include "../resources.h"

class GuiTreeBoxItem
{
public:
   GuiTreeBoxItem();
   ~GuiTreeBoxItem();

 // get/set interface
   const std::string&   getText() const;
         void           setText(const std::string& text);

         bool           hasIcon() const;
         TexturePtr     getIcon();

         bool           isSelected() const;
         void           setSelected(bool selected);

         void*          getUserData();
         void           setUserData(void* puserdata);

 // operations
         void setIcon(const std::string& icon);

private:
   TexturePtr  _icon;
   std::string _text;
   void*       _puserdata;
   bool        _selected;
};

#ifdef JENGINE_INLINE
#  include "guitreeboxitem.inl"
#endif

#endif
