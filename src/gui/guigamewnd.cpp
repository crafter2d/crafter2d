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
#include "guigamewnd.h"
#ifndef JENGINE_INLINE
#  include "guigamewnd.inl"
#endif

#include "../containers/listiterator.h"

#include "../world/isometricrendervisitor.h"
#include "../world/layer.h"
#include "../world/world.h"

#include "../pickvisitor.h"
#include "../script.h"
#include "../scriptmanager.h"
#include "../sound.h"
#include "../sortedobjectlist.h"
#include "../player.h"
#include "../game.h"

GuiGameWnd::GuiGameWnd(void):
   zoom(300.0f)
{
}

void GuiGameWnd::onRender (Uint32 tick, const GuiGraphics& graphics)
{
   Client& client = Game::getInstance().getClient();
   //if ( client.isInitialized() )
   {
      SceneGraph& graph = client.getSceneGraph();

      if ( graph.hasWorld() )
      {
         // set the sound of the player
         Object* pcontroler = graph.getControler();
         if ( pcontroler != NULL )
            SoundManager::setPlayerPosition( pcontroler->getPosition() );

         // draw the scenegraph
         glColor4f(1,1,1,1);

         renderWorld(graph);

         // see if there is any creature under the mouse
         //pick(tick, graph);

         // reset the matrix
         glLoadIdentity ();
      }
   }

   renderChildren(tick, graphics);
}

void GuiGameWnd::renderWorld(SceneGraph& graph)
{
   graph.getWorld()->draw(true);

   /*
   IsometricRenderVisitor visitor;
   visitor.traverse(graph.getRoot());

   SortedObjectList& list = visitor.getObjects();
   for ( ListIterator<Object> it(list); it.isValid(); ++it )
   {
      Object& object = it.item();
      object.draw();
   }
   */
}

int GuiGameWnd::onLButtonUp(const GuiPoint& point, int flags)
{
   SceneGraph& graph = Game::getInstance().getClient().getSceneGraph();
   doPick(graph, true);

   return JENGINE_MSG_HANDLED;
}

void GuiGameWnd::onMouseWheel (const GuiPoint& point, int direction, int flags)
{
   if (direction == -1)
      zoom -= 10;
   else
      zoom += 10;
}

void GuiGameWnd::onActivate (bool active, GuiWnd* oldWnd)
{
   GuiWindow::onActivate(active, oldWnd);
   if (active)
      ScriptManager::getInstance().executeLine("GuiGameWnd_onActivate(1)");
   else
      ScriptManager::getInstance().executeLine("GuiGameWnd_onActivate(0)");
}

void GuiGameWnd::pick(Uint32 tick, SceneGraph& graph)
{
   const Uint32 PICK_INTERVAL = 25;
   static Uint32 last = tick;

   if (tick - last > PICK_INTERVAL)
   {
      doPick(graph, false);

      last = tick;
   }
}

void GuiGameWnd::doPick(SceneGraph& graph, bool click)
{
   if ( graph.getWorld() != NULL && graph.getWorld()->getLayerCount() > 0 )
   {
      int x,y;

      // pick any object which is under the mouse
      SDL_GetMouseState(&x,&y);
      Vector mousePos(static_cast<float>(x), static_cast<float>(y));
      mousePos = graph.getWorld()->getLayer(0)->screenToLayer(mousePos);

      PickVisitor pv(mousePos);
      pv.traverse(graph.getRoot());
      Object* picked = pv.getPicked();
      GuiPoint pos(x,y);

      // let the script know that the player has the mouse over an object
      Script& script = ScriptManager::getInstance().getTemporaryScript();
      script.prepareCall("Client_onPick");
      script.addParam(picked, "Object");
      script.addParam(&pos, "GuiPoint");
      script.addParam(click);
      script.run(3);
   }
}
