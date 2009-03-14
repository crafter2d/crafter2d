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
#ifndef GUI_LAYER_EDITOR_H_
#define GUI_LAYER_EDITOR_H_

#include "guihelpers.h"

class Layer;

class GuiTileEditorLayer
{
public:
   static GuiTileEditorLayer*  construct(Layer& layer);

   explicit GuiTileEditorLayer(Layer& layer);
   virtual ~GuiTileEditorLayer() = 0;

 // get/set interface
   Layer& getLayer();

 // operations
   virtual GuiPoint pointToTile(const GuiPoint& point) = 0;

private:
   Layer& _layer;
};

#ifdef JENGINE_INLINE
#  include "guitileeditorlayer.inl"
#endif

#endif
