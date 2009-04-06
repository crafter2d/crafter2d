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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL/SDL.h>
#include <string>

#include "gui/guicanvas.h"

#include "actionmap.h"
#include "server.h"
#include "client.h"
#include "gamesettings.h"

#define DECLARE_APPLICATION(title) \
   const char* getTitle() { return title; }

#define IMPLEMENT_APPLICATION(className) \
   Game& Game::getInstance() {           \
      static className game;             \
      return game;                       \
   }


class GuiDesigner;
class GameConfiguration;
class Timer;
class TimerData;

/**
@author Jeroen Broekhuizen
\brief This is the main application class. You must inherit your own class from it. It implements the
complete rendering and event loop. See the initGame, endGame and runFrame functions for more information.
*/
class Game
{
public:
 // static interface
   static Game&         getInstance();

                        Game();
   virtual              ~Game() = 0;

 // get/set interface
         bool                 designing() const;
         void                 designing(bool designing);

   const GameConfiguration&   getConfiguration() const;
         void                 setConfiguration(const GameConfiguration& pconfiguration);

         void                 setActive(bool act=true);
         bool                 isActive() const;

         void                 setTitle(const std::string& title);
   const std::string&         getTitle() const;

         Server&              getServer();
         Client&              getClient();
         GuiCanvas&           getCanvas();
         GuiDesigner&         designer();

 // operations
   bool                 create();
   void                 destroy();
   void                 processFrame();
   void                 run();

 // query
   void                 getWindowDimensions(int& width, int& height);

   int                  getScreenWidth() const;
   int                  getScreenHeight() const;

 // overloads
   virtual void         loadCustomScriptLibraries();

protected:
 // get/set
   TimerData&           getTimerData();

 // overloadables
   virtual bool         initGame();
   virtual void         endGame();
   virtual void         updateFrame(Uint8* keys);
   virtual void         drawFrame(Uint32 tick) = 0;

   virtual bool         onResize(int width, int height);

   void                 onKeyboardEvent(const SDL_KeyboardEvent& event);
   void                 onMouseButtonEvent(const SDL_MouseButtonEvent& event);
   void                 onMouseMoveEvent(const SDL_MouseMotionEvent& event);

   bool        active;
   bool        keyboardEventsEnabled;
   int         width;
   int         height;
   GuiCanvas   canvas;
   Server      server;
   Client      client;

private:
   void                 initOpenGL();
   int                  preConfigScreen();
   void                 runFrame();
   int                  mouseKeyFlags();

   void                 switchDesigner();
   void                 switchEditor();

   GameSettings         mSettings;
   GuiColor             mGlClearColor;
   std::string          mTitle;
   GameConfiguration*   mpConfiguration;     // owned
   SDL_Surface*         window;
   GuiDesigner*         mpDesigner;
   TimerData*           mpTimerData;
   int                  bitdepth;
   int                  videoFlags;
   bool                 mDesigning;
};

#ifdef JENGINE_INLINE
#  include "game.inl"
#endif

#endif
