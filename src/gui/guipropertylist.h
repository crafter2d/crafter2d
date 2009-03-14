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
#ifndef GUIPROPERTYLIST_H
#define GUIPROPERTYLIST_H

#include "guilistbox.h"

class GuiProperties;
class GuiPropertyBase;

/**
@author Jeroen Broekhuizen
*/
class GuiPropertyList : public GuiListBox
{
public:
           GuiPropertyList();
   virtual ~GuiPropertyList();

   const GuiProperties& Properties() const;
         GuiProperties& Properties();
         void           Properties(GuiProperties& pproperties);

         void           window(GuiWnd* pwnd);

 // overloads
   virtual void      applyChange(const std::string& text);
   virtual EditType  editType(int index) const;
   virtual void      fillCombo(int row);

private:
   void  addProperty(GuiPropertyBase& pproperty, int propertyindex);

   GuiProperties* MPProperties;
   GuiWnd*        _pwindow;
};

#endif
