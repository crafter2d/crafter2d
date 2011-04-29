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
#include "game.h"
#ifndef JENGINE_INLINE
#  include "game.inl"
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vfw.h>
#endif

#include <iostream>
#include <cstdlib>
#include <GL/GLee.h>
#include <GL/glu.h>
#include <tolua++.h>

#include "core/script/scriptcontext.h"
#include "core/script/scriptmanager.h"

#include "core/system/platform.h"
#include "core/system/timer.h"
#include "core/system/timerdelta.h"

#include "core/vfs/filesystem.h"

#include "core/smartptr/autoptr.h"

#include "engine/tools/profiler/profiler.h"
#include "engine/tools/profiler/profilerinstance.h"

#include "engine/physics/physicsfactory.h"
#include "engine/physics/box2d/box2dfactory.h"
#include "engine/physics/simulationfactoryregistry.h"

#include "engine/net/netobjectfactory.h"
#include "engine/net/netconnection.h"

#include "engine/client.h"

#include "engine/opengl.h"

#include "console.h"
#include "gamesettings.h"
#include "tolua_game.h"

/*!
    \fn Game::Game()
	 \brief Initialized member variables
 */
Game::Game():
   mWindow(),
   mWindowListener(*this),
   mTitle(),
   mpTimerData(NULL),
   mScriptManager(),
   mActive(true)
{
}

/*!
    \fn Game::~Game()
	 \brief Calls destroy when game is finished.
 */
Game::~Game()
{
   try
   {
      destroy();
   }
   catch (...)
   {
      // ignore, we are shutting down anyways
   }
}

/*!
    \fn Game::create(char* caption, int w, int h, int bd)
	 \brief Call this function to initialize the main window and OpenGL for your game.
	 \param[in] caption caption of the window
	 \param[in] w width of the window
	 \param[in] h height of the window
	 \param[in] bd color bitdepht of screen (16,24,32)
	 \return true if initialized successfull, false otherwise
 */

#include "script/as.h"
#include "core/string/string.h"

bool Game::create()
{
   String s("hoi");
   std::string m = s.toStdString();

   Log& log = Log::getInstance();
   log << "JEngine SSE V0.4.5 - Copyright 2010 - Jeroen Broekhuizen\n";
   log << "Released under LGPL, see license.txt file for more info.\n";
   log << "---------------------------------------------------------\n";

   runScript();

   // initialize the video library
   if (SDL_Init (SDL_INIT_VIDEO) < 0) {
      log << "Couldn't initialize the SDL library!";
      return false;
   }

   mpTimerData = TIMER.createData();

   // initialize the Lua scripting environment
   mScriptManager.initialize ();
   mScriptManager.loadModule(tolua_game_open);

   // register the physics factory
   SimulationFactoryRegistry::getInstance().addFactory(new PhysicsFactory());
   SimulationFactoryRegistry::getInstance().addFactory(new Box2DFactory());

   log << "\n-- Initializing Graphics --\n\n";

   mWindow.addListener(mWindowListener);
   if ( !mWindow.create(mTitle, 800, 600, 32, false) )
   {
      return false;
   }

   // now initialize OpenGL for rendering
   if ( !initOpenGL() )
   {
      return false;
   }   

   // initialize the console
   Console& console = Console::getInstance();
   console.create ();

#ifdef WIN32
   NetConnection::initialize();

   // initialize the AVI library
   AVIFileInit();
#endif

   NetObjectFactory::getInstance().initialize();

   log << "\n-- Initializing Sound --\n\n";

   log << "\n-- Running Game --\n\n";

   // reload the contents of the log file for the console
   console.reload();

   FileSystem::getInstance().addPath("..");

   ScriptContext context;
   mScriptManager.executeScript(context, "scripts/main.lua");

   // give the game time to load in stuff before window shows up
   // (after that, the game has to keep track of it's own state)
   if ( !initGame () )
   {
      console.error("Aborted after failed game initialization.");
      return false;
   }

   return true;
}

/*!
    \fn Game::destroy()
	 \brief When finished this function will be called automatically by the destructor.
	 \return Nothing
 */
void Game::destroy()
{
   // free the game resources
	endGame ();

	// release the Lua scripting environment
	mScriptManager.destroy ();

	// release the sound syste

   // release timer data
   TIMER.releaseData(mpTimerData);

#ifdef WIN32
   // release the avi library
   AVIFileExit();
#endif

   mWindow.destroy();

	// finish of SDL
	SDL_Quit ();
}

//////////////////////////////////////////////////////////////////////////
// - Notifications
//////////////////////////////////////////////////////////////////////////

void Game::onWindowResized()
{
   // set the new opengl states
   glViewport(0, 0, mWindow.getWidth(), mWindow.getHeight());

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, mWindow.getWidth(), mWindow.getHeight(), 0, 0, 1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void Game::onWindowClosed()
{
  setActive(false);
}

//////////////////////////////////////////////////////////////////////////
// - operations
//////////////////////////////////////////////////////////////////////////

/*!
    \fn Game::initOpenGL()
 */
bool Game::initOpenGL()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel (GL_SMOOTH);

	return OpenGL::initialize ();
}

/*!
    \fn Game::run()
	 \brief Runs the main event loop of the game.
	 \return Nothing
 */
void Game::run()
{
   TIMER.start(getTimerData());

	while ( mActive )
   {
		processFrame();
	}
}

void Game::processFrame()
{
   mWindow.handleEvents();

   runFrame();
}

/*!
   \fn Game::getWindowDimensions(int& w, int& h)
   \brief Returns the current dimensions of the window.
   \param w The width of the window
   \param h The height of the window
*/
void Game::getWindowDimensions(int& w, int& h)
{
   w = mWindow.getWidth();
   h = mWindow.getHeight();
}

/*!
    \fn Game::initGame()
	 \brief This function will be called during the initialization phase of the game. The window and OpenGL are both
	 already initialized. Overload it to customize it to your needs.
	 \return Return false to quit the game, or true if initialization was succesfull
 */
bool Game::initGame()
{
   ScriptContext context;
   Script& script = mScriptManager.getTemporaryScript();
   if ( script.prepareCall("Game_initialize") )
   {
      //script.setSelf(this, "Game");
      script.run(context);
   }

   /*
   // initialize the window manager
   GuiManager& manager = GuiManager::getInstance();
   manager.initialize();

   GuiFont* font = new GuiFont ();
   font->initialize ("amersn.ttf", 10);

   manager.setDefaultFont (font);
   manager.setDefaultTextColor(mSettings.getTextColor());

   // create the gui canvas
   mCanvas.create(0, GuiRect(0, mWindow.getWidth(), 0, mWindow.getHeight()));
   mCanvas.changeDefaultColor(GuiCanvas::GuiWindowColor, mSettings.getWindowColor());
   mCanvas.changeDefaultColor(GuiCanvas::GuiBorderColor, mSettings.getBorderColor());

   ScriptManager& scriptMgr = ScriptManager::getInstance ();
   scriptMgr.setObject(&GuiManager::getInstance(), "GuiManager", "guimanager");
   scriptMgr.setObject(&GuiFocus::getInstance(), "GuiFocus", "focus");
   */

	return mActive;
}

/*!
    \fn Game::endGame()
	 \brief When the game is closed this function will be called to allow you to free any allocated resources
	 and shutdown properly.
	 \return Nothing
 */
void Game::endGame()
{
   Client client;

	ScriptContext context;
   Script& script = mScriptManager.getTemporaryScript();
   script.setSelf(&client, "Client");
   script.prepareCall("Game_shutdown");
   script.setSelf(this, "Game");
   script.run(context);
}

/*!
    \fn Game::runFrame()
	 \brief Called when next frame should be rendered. Overload it to render your own custom objects.
	 \returns Nothing
 */
void Game::runFrame()
{
   Profiler::getInstance().begin();

   TimerDelta timerdelta(getTimerData());
   float delta = timerdelta.getDelta();

   ScriptContext context;
   mScriptManager.update(context, delta);
   if ( !isActive() )
      return;

   Script& script = mScriptManager.getTemporaryScript();
   script.prepareCall("Game_run");
   script.addParam(delta);
   script.run(context, 1);
   
   // here also nothing happens (should be overloaded)
   glLoadIdentity ();
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glAlphaFunc (GL_GREATER, 0.1f);
   glEnable (GL_ALPHA_TEST);

   //glDisable(GL_MULTISAMPLE);
   glDisable (GL_ALPHA_TEST);

   Profiler::getInstance().end();
   // Profiler::getInstance().draw(*GuiManager::getInstance().getDefaultFont());

   SDL_GL_SwapBuffers ();
}
