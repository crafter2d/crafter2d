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

#include "gui/canvas/guicanvas.h"

#include "actionmap.h"
#include "server.h"
#include "client.h"
#include "gamesettings.h"
#include "gamewindow.h"
#include "defaultgamewindowlistener.h"

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
class WorldRenderer;

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
   const GameConfiguration&   getConfiguration() const;
         void                 setConfiguration(const GameConfiguration& pconfiguration);

         GameWindow&          getGameWindow();

         void                 setActive(bool act=true);
         bool                 isActive() const;

         void                 setTitle(const std::string& title);
   const std::string&         getTitle() const;

         Server&              getServer();
         Client&              getClient();
         GuiCanvas&           getCanvas();

 // operations
   bool                 create();
   void                 destroy();
   void                 processFrame();
   void                 run();

 // query
   void                 getWindowDimensions(int& width, int& height);

   int                  getScreenWidth() const;
   int                  getScreenHeight() const;

  // notifications
   void onWindowResized();
   void onWindowClosed();

 // overloads
   virtual void         loadCustomScriptLibraries();

protected:
 // get/set
   TimerData&           getTimerData();

 // overloadables
   virtual bool         initGame();
   virtual void         endGame();
   virtual void         drawFrame(float delta) = 0;

   void                 onKeyboardEvent(const SDL_KeyboardEvent& event);
   void                 onMouseButtonEvent(const SDL_MouseButtonEvent& event);
   void                 onMouseMoveEvent(const SDL_MouseMotionEvent& event);

   bool        active;
   Server      server;
   Client      client;

private:
   void                 initOpenGL();
   void                 runFrame();

   GameSettings              mSettings;
   GameConfiguration*        mpConfiguration;     // owned
   GameWindow                mWindow;
   DefaultGameWindowListener mWindowListener;
   std::string               mTitle;
   GuiCanvas                 mCanvas;
   TimerData*                mpTimerData;
};

#ifdef JENGINE_INLINE
#  include "game.inl"
#endif

#endif
