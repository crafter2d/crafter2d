/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public Licen5se as published by  *
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

/* This file contains the prototypes for the toLua program to bind these
 * classes to Lua. GUI part of the classes.
 */

$#include "world/world.h"
$#include "gui/guihelpers.h"
$#include "gui/guiimagelistbox.h"
$#include "gui/guitileeditor.h"

class GuiImageListBox : public GuiListBox
{
   GuiImageListBox();

   int   getActiveLayer() const;
   void  setActiveLayer(int activeLayer);

   void  world(const World* pworld);
};

class GuiTileEditor : public GuiControl
{
   GuiTileEditor();

   // get/set interface
   int getActiveLayer() const;
   void setActiveLayer(int activeLayer);

   bool    hasWorld() const;
   World&  world();
   void    world(World* pworld);

   // operations
   GuiPoint PointToTile(const GuiPoint& point);
   void     showAll(bool all);
   Bound*   pickBound(const GuiPoint& point);
};
