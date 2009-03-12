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
#ifndef GUI_TILEEDITOR_WORLD_OBSERVER_H_
#define GUI_TILEEDITOR_WORLD_OBSERVER_H_

#include "../worldobserver.h"

class GuiTileEditor;

class GuiTileEditorWorldObserver : public WorldObserver
{
public:
   explicit GuiTileEditorWorldObserver(GuiTileEditor& editor);
   virtual  ~GuiTileEditorWorldObserver();

   virtual void notifyLayerAdded(Layer& layer);

private:
   GuiTileEditor& _editor;
};

#endif
