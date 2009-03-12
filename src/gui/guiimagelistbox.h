/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

#ifndef GUI_IMAGELISTBOX_H_
#define GUI_IMAGELISTBOX_H_

#include <assert.h>
#include "guilistbox.h"

class GuiGraphics;
class World;

class GuiImageListBox : public GuiListBox
{
public:
   DESIGNER_REGISTRATION(GuiImageListBox)

   GuiImageListBox();

   bool           hasActiveLayer() const;
   int            getActiveLayer() const;
   void           setActiveLayer(int activeLayer);

   const World&   world() const;
   void           world(const World* world);

protected:
   virtual int    measureItem(int index);
   virtual void   drawItem(int index, int posX, int posY, const GuiGraphics& graphics);

   const GuiImageListBox&  me();

private:
   const World*   MPWorld;
   int            _activeLayer;
};

#ifdef JENGINE_INLINE
#  include "guiimagelistbox.inl"
#endif

#endif
