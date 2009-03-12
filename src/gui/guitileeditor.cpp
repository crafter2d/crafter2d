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
#include "guitileeditor.h"
#ifndef JENGINE_INLINE
#  include "guitileeditor.inl"
#endif

#include "../world/layer.h"
#include "../world/bound.h"
#include "../world/world.h"

#include "../script.h"
#include "../scriptmanager.h"

#include "guidesigner.h"
#include "guihelpers.h"
#include "guiscrollbar.h"
#include "guieventhandler.h"
#include "guieventhandlers.h"
#include "guieventhandlerdefinition.h"
#include "guieventhandlerdefinitions.h"
#include "guiscopedtransform.h"

REGISTER_DESIGNER(GuiTileEditor, GuiTileEditorId, "Tile Editor" , 40, 15, 264)

EVENT_MAP_BEGIN(GuiTileEditor)
   ON_SB_UP(1, onHorzScroll)
   ON_SB_DOWN(1, onHorzScroll)
   ON_SB_UP(2, onVertScroll)
   ON_SB_DOWN(2, onVertScroll)
EVENT_MAP_END()

GuiTileEditor::GuiTileEditor():
   GuiControl(),
   _pworld(0),
   _layers(),
   _selectedBounds(),
   MPHorzScrollBar(NULL),
   MPVertScrollBar(NULL),
   _worldObserver(*this),
   _highlightTile(),
   MXScrollPos(0),
   MYScrollPos(0),
   _activeLayer(0),
   _showAll(false)
{
}

//////////////////////////////////////////////////////////////////////////
// - Creation interface
//////////////////////////////////////////////////////////////////////////

void GuiTileEditor::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiControl::onCreate(rect, caption, style, parent);

   initializeScrollbars();
}

void GuiTileEditor::initializeEventHandlerDefinitions()
{
   GuiControl::initializeEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiTileEditorMouseDownEvent, "onMouseDown");
   pdefinition->addArgument("point");
   getEventHandlerDefinitions().add(pdefinition);

   pdefinition = new GuiEventHandlerDefinition(GuiTileEditorMouseClickEvent, "onClick");
   pdefinition->addArgument("point");
   getEventHandlerDefinitions().add(pdefinition);

   pdefinition = new GuiEventHandlerDefinition(GuiTileEditorMouseMoveEvent, "onMouseMove");
   pdefinition->addArgument("point");
   pdefinition->addArgument("relative");
   pdefinition->addArgument("pressed");
   getEventHandlerDefinitions().add(pdefinition);

   pdefinition = new GuiEventHandlerDefinition(GuiTileEditorKeyPressEvent, "onKeyPress");
   pdefinition->addArgument("key");
   getEventHandlerDefinitions().add(pdefinition);
}

void GuiTileEditor::initializeScrollbars()
{
   const GuiRect& rect = getWindowRect();
   int w = rect.getWidth();
   int h = rect.getHeight();

   if ( MPHorzScrollBar != NULL )
   {
      removeChild(MPHorzScrollBar);
      delete MPHorzScrollBar;
   }
   MPHorzScrollBar = new GuiScrollBar();
   MPHorzScrollBar->create(1, GuiRect(1, w-18, h-18, h), "", GUI_VISIBLE|GUI_BACKGROUND|GUI_HORZ_SCROLL, this);
   MPHorzScrollBar->setScrollInfo(15);
   MPHorzScrollBar->setScrollRange(1);

   if ( MPVertScrollBar != NULL )
   {
      removeChild(MPVertScrollBar);
      delete MPVertScrollBar;
   }
   MPVertScrollBar = new GuiScrollBar();
   MPVertScrollBar->create(2, GuiRect(w-15, w, 1, h-18), "", GUI_VISIBLE|GUI_BACKGROUND, this);
   MPVertScrollBar->setScrollInfo(15);
   MPVertScrollBar->setScrollRange(1);
}

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

bool GuiTileEditor::hasActiveLayer() const
{
   return getActiveLayer() >= 0 && getActiveLayer() < world().getLayerCount();
}

void GuiTileEditor::world(World* pworld)
{
   if ( _pworld != NULL )
   {
      for ( int index = 0; index < _layers.size(); ++index )
      {
         delete _layers[index];
      }
      _layers.clear();

      _pworld->detach(_worldObserver);
   }

   _pworld = pworld;
   _pworld->attach(_worldObserver);

   for ( int index = 0; index < world().getLayerCount(); ++index )
   {
      Layer* player = world().getLayer(index);

      GuiTileEditorLayer* playerEditor = GuiTileEditorLayer::construct(*player);
      _layers.push_back(playerEditor);

      player->prepare(getWindowRect().getWidth(), getWindowRect().getHeight());
   }

   setActiveLayer(0);
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiTileEditor::paint (Uint32 tick, const GuiGraphics& graphics)
{
   if ( hasWorld() )
   {
      GuiScopedTransform transform(*this);

      if ( hasActiveLayer() )
      {
         if ( _showAll )
         {
            for ( int i = 0; i < world().getLayerCount(); ++i )
            {
               Layer& layer = *world().getLayer(i);
               paintLayer(layer);
            }
         }
         else
         {
            Layer& layer = _layers[getActiveLayer()]->getLayer();
            paintLayer(layer);

            //layer.drawHighlight(Vector(_highlightTile.x, _highlightTile.y));
         }
      }

      paintBounds(graphics);
      paintObjects();
   }
}

void GuiTileEditor::paintLayer(Layer& layer)
{
   const Effect& effect = layer.getEffect();
   effect.enable();

   layer.draw();

   effect.disable();
}

void GuiTileEditor::paintBounds(const GuiGraphics& graphics)
{
   graphics.setColor(1,0,1);

   if ( hasActiveLayer() )
   {
      Layer* player = world().getLayer(getActiveLayer());
      Vector scroll = player->getScroll();

      for ( int index = 0; index < world().getBoundCount(); ++index )
      {
         Bound& bound = world().getBound(index);

         graphics.drawLine(GuiPoint(bound.getLeft().x - scroll.x, bound.getLeft().y - scroll.y),
                           GuiPoint(bound.getRight().x - scroll.x, bound.getRight().y - scroll.y));

         Vector center = ((bound.getLeft() + bound.getRight()) / 2);
         Vector other  = center + (bound.getNormal() * 5) - scroll;
         center -= scroll;

         graphics.drawLine(GuiPoint(center.x, center.y), GuiPoint(other.x, other.y));
      }
   }
}

void GuiTileEditor::paintObjects()
{
   if ( world().getLayerCount() > 0 && world().getChildren().size() > 0 )
   {
      ScopedTransform transform(-world().getLayer(0)->getScroll());

      SceneObject::SceneObjectList::const_iterator it = world().getChildren().begin();
      for ( ; it != world().getChildren().end(); ++it )
      {
         SceneObject& object = *const_cast<SceneObject*>(*it);
         object.draw();
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// - Scrolling interface
//////////////////////////////////////////////////////////////////////////

void GuiTileEditor::onHorzScroll(int pos)
{
   Layer* player = world().getLayer(getActiveLayer());
   Vector scroll = player->getScroll();

   player->setScroll(pos * player->tilewidth(), scroll.y);

   MXScrollPos = pos;
}

void GuiTileEditor::onVertScroll(int pos)
{
   Layer* player = world().getLayer(getActiveLayer());
   Vector scroll = player->getScroll();

   player->setScroll(scroll.x, pos * player->tileheight());

   MYScrollPos = pos;
}

void GuiTileEditor::onResize(int width, int height)
{
   initializeScrollbars();
}

//////////////////////////////////////////////////////////////////////////
// - Input interface
//////////////////////////////////////////////////////////////////////////

int GuiTileEditor::onLButtonDown(const GuiPoint& point, int flags)
{
   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiTileEditorMouseDownEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiTileEditor");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam((void*)&point, "GuiPoint");
      script.run(1);
   }

   return JENGINE_MSG_HANDLED;
}

int GuiTileEditor::onLButtonUp(const GuiPoint& point, int flags)
{
   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiTileEditorMouseClickEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiTileEditor");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam((void*)&point, "GuiPoint");
      script.run(1);
   }

   return JENGINE_MSG_HANDLED;
}

void GuiTileEditor::onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flags)
{
   _highlightTile = PointToTile(point);

   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiTileEditorMouseMoveEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiTileEditor");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam((void*)&point, "GuiPoint");
      script.addParam((void*)&rel, "GuiPoint");
      script.addParam((bool)(flags & GuiLButton));
      script.run(3);
   }
}

void GuiTileEditor::onKeyUp(int which)
{
   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiTileEditorKeyPressEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiTileEditor");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam(which);
      script.run(1);
   }
}

//////////////////////////////////////////////////////////////////////////
// - Notification interface
//////////////////////////////////////////////////////////////////////////

void GuiTileEditor::notifyLayerAdded(Layer& layer)
{
   GuiTileEditorLayer* playerEditor = GuiTileEditorLayer::construct(layer);
   _layers.push_back(playerEditor);
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

GuiPoint GuiTileEditor::PointToTile(const GuiPoint& point)
{
   if ( hasWorld() && hasActiveLayer() )
      return GuiPoint(_layers[getActiveLayer()]->getLayer().pointToTile((Point)point));
   else
      return GuiPoint(0,0);
}

void GuiTileEditor::setActiveLayer(int activeLayer)
{
   _activeLayer = activeLayer;

   Vector dimensions;
   if ( hasActiveLayer() )
   {
      Layer* layer = world().getLayer(getActiveLayer());
      dimensions = layer->getDimensions();

      Vector scroll = layer->getScroll();
      MPHorzScrollBar->setScrollPosition(scroll.x / layer->tilewidth());
      MPVertScrollBar->setScrollPosition(scroll.y / layer->tileheight());
   }

   MPHorzScrollBar->setScrollRange(dimensions.x);
   MPVertScrollBar->setScrollRange(dimensions.y);
}

Bound* GuiTileEditor::pickBound(const GuiPoint& point)
{
   Layer* player = world().getLayer(getActiveLayer());
   Vector scroll = player->getScroll();

   float distance = 0.0f;

   for ( int index = 0; index < world().getBoundCount(); ++index )
   {
      Bound& bound = world().getBound(index);
      if ( bound.hitTest(Vector(point.x + scroll.x, point.y + scroll.y), distance) )
      {
         if ( distance <= 1.0f )
            return &bound;
      }
   }

   return NULL;
}
