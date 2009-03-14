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
#ifndef GUIMENU_H
#define GUIMENU_H

#include <vector>
#include "guicontrol.h"
#include "guimenuitem.h"

/**
@author Jeroen Broekhuizen
*/
class GuiMenu : public GuiControl
{
public:
   typedef std::vector<GuiMenuItem> GuiMenuItems;
   
            GuiMenu();
   virtual ~GuiMenu();

   const GuiPoint&   getLocation() const;
   void              setLocation(const GuiPoint& location);

   int               getSelection() const;
   void              setSelection(int selection);

   bool                 hasItems() const;
   const GuiMenuItems&  getItems() const;
         GuiMenuItems&  getItems();
   
   virtual void     addItem(int id, const std::string& item);
   void             calculateSize();
   
   virtual void   onKillFocus(GuiWnd* newCtrl);
   virtual void   onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flags);
   virtual int    onLButtonUp(const GuiPoint& point, int flag);

protected:
   virtual void   onRender(Uint32 tick, const GuiGraphics& graphics);

   virtual void   initializeEventHandlerDefinitions();

   void selectItem(const GuiPoint& point);

private:
   GuiMenuItems MItems;
   std::string  _name;
   GuiPoint     _popupLocation;
   int          _selection;
   bool         _processing;
};

#ifdef JENGINE_INLINE
#  include "guimenu.inl"
#endif

#endif
