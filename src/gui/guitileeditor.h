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
#ifndef GUI_TILEEDITOR_H_
#define GUI_TILEEDITOR_H_

#include "guicontrol.h"
#include "guitileeditorlayer.h"
#include "guitileeditorworldobserver.h"

#include "tileeditor/tileeditormouselistener.h"
#include "tileeditor/tileeditormousemotionlistener.h"

class GuiScrollBar;
class World;
class Bound;

class GuiTileEditor : public GuiControl
{
public:
   DESIGNER_REGISTRATION(GuiTileEditor)

   typedef std::vector<GuiTileEditorLayer*>  Layers;
   typedef std::vector<Bound*>               Bounds;

   GuiTileEditor();

  // get/set interface
   bool           hasActiveLayer() const;
   int            getActiveLayer() const;
   void           setActiveLayer(int activeLayer);

   void           showAll(bool all);

   bool           hasWorld() const;
   const World&   world() const;
         World&   world();
   void           world(World* pworld);

   // operations
   virtual void   sendMessage(GuiId id, GuiEvent event, int param1);

   Bound*         pickBound(const GuiPoint& point);

   GuiPoint       PointToTile(const GuiPoint& point);

protected:
   virtual void onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void paint (Uint32 tick, const GuiGraphics& graphics);
   virtual void onResize(int width, int height);

   //virtual int  onLButtonDown(const GuiPoint& point, int flags);
   //virtual int  onLButtonUp(const GuiPoint& point, int flags);
   //virtual void onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flags);
   virtual void onKeyUp(int which);

   virtual void initializeEventHandlerDefinitions();

  // messages
   void         onHorzScroll(int pos);
   void         onVertScroll(int pos);

  // initialization
   void         initializeScrollbars();

  // painting
   void         paintLayer(Layer& layer);
   void         paintBounds(const GuiGraphics& graphics);
   void         paintObjects();

private:
   friend class GuiTileEditorWorldObserver;

   void notifyLayerAdded(Layer& layer);

   const GuiTileEditor& me();

   World*         _pworld;
   Layers         _layers;
   Bounds         _selectedBounds;
   GuiScrollBar*  MPHorzScrollBar;
   GuiScrollBar*  MPVertScrollBar;
   
   GuiTileEditorWorldObserver _worldObserver;
   
   TileEditorMouseListener       mMouseListener;
   TileEditorMouseMotionListener mMouseMotionListener;

   GuiPoint       _highlightTile;
   int            MXScrollPos;
   int            MYScrollPos;
   int            _activeLayer;
   bool           _showAll;
};

#ifdef JENGINE_INLINE
#  include "guitileeditor.inl"
#endif

#endif // GUI_TILEEDITOR_H_
