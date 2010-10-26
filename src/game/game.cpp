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
#include <vfw.h>
#endif

#include <iostream>
#include <cstdlib>
#include <GL/GLee.h>
#include <GL/glu.h>
#include <tolua++.h>

#include "tools/profiler/profiler.h"
#include "tools/profiler/profilerinstance.h"

#include "physics/physicsfactory.h"
#include "physics/box2d/box2dfactory.h"
#include "physics/simulationfactoryregistry.h"

#include "net/netobjectfactory.h"
#include "net/netconnection.h"

#include "system/platform.h"
#include "system/timer.h"
#include "system/timerdelta.h"

#include "console.h"
#include "opengl.h"
#include "scriptmanager.h"
#include "creature.h"
#include "gameconfiguration.h"
#include "gameconfigurationselector.h"
#include "gamesettings.h"

/*!
    \fn Game::Game()
	 \brief Initialized member variables
 */
Game::Game():
   mSettings(),
   mpConfiguration(NULL),
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
bool Game::create()
{
   Log& log = Console::getLog();
   log << "JEngine SSE V0.4.5 - Copyright 2010 - Jeroen Broekhuizen\n";
   log << "Released under LGPL, see license.txt file for more info.\n";
   log << "---------------------------------------------------------\n";

   // initialize the video library
   if (SDL_Init (SDL_INIT_VIDEO) < 0) {
      log << "Couldn't initialize the SDL library!";
      return false;
   }

   mpTimerData = TIMER.createData();

   // initialize the Lua scripting environment
   mScriptManager.initialize ();
   mScriptManager.setObject(&Console::getInstance(), "Console", "console");

   // load the engine settings
   mSettings.initialize();

   // register the physics factory
   SimulationFactoryRegistry::getInstance().addFactory(new PhysicsFactory());
   SimulationFactoryRegistry::getInstance().addFactory(new Box2DFactory());

   log << "\n-- Initializing Graphics --\n\n";

   mWindow.addListener(mWindowListener);
   if ( !mWindow.create(mTitle, mSettings.getWidth(), mSettings.getHeight(), mSettings.getBitDepth(), mSettings.getFullScreen()) )
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

   // give the game time to load in stuff before window shows up
   // (after that, the game has to keep track of it's own state)
   if ( !initGame () )
   {
      console.error("Aborted after failed game initialization.");
      return false;
   }

   mScriptManager.executeScript("main.lua");

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
// - get/set
//////////////////////////////////////////////////////////////////////////

void Game::setConfiguration(const GameConfiguration& configuration)
{
   delete mpConfiguration;
   mpConfiguration = configuration.clone();
}

//////////////////////////////////////////////////////////////////////////
// - operations
//////////////////////////////////////////////////////////////////////////

/*!
    \fn Game::initOpenGL()
 */
bool Game::initOpenGL()
{
   const GuiColor& clearcolor = mSettings.getClearColor();
   glClearColor(clearcolor.r, clearcolor.g, clearcolor.b, 0.0f);
	//glClearDepth(1.0f);

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
   Script& script = mScriptManager.getTemporaryScript();
   script.prepareCall("game_initialize");
   script.setSelf(this, "Game");
   script.addParam(delta);
   script.run(1);

   // select the configuration
   GameConfigurationSelector selector(*this);
   if ( selector.select() )
   {
      setConfiguration(selector.getSelectedConfiguration());

      Console::getInstance().printf("Using configuration %s", getConfiguration().getName().c_str());
   }
   else
   {
      Console::getInstance().error("No configuration selected, aborting engine.");
      return false;
   }

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
	// do nothing (should be overloaded)
}

/*!
    \fn Game::runFrame()
	 \brief Called when next frame should be rendered. Overload it to render your own custom objects.
	 \returns Nothing
 */
void Game::runFrame()
{
   Uint32 tick = SDL_GetTicks ();

   Profiler::getInstance().begin();

   TimerDelta timerdelta(getTimerData());
   float delta = timerdelta.getDelta();

   mScriptManager.update(tick);
   if ( !isActive() )
      return;

   Script& script = mScriptManager.getTemporaryScript();
   script.prepareCall("game_run");
   script.setSelf(this, "Game");
   script.addParam(delta);
   script.run(1);
   
   // here also nothing happens (should be overloaded)
   glLoadIdentity ();
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glAlphaFunc (GL_GREATER, 0.1f);
   glEnable (GL_ALPHA_TEST);

   // call overloaded function
   drawFrame(delta);

   //glDisable(GL_MULTISAMPLE);
   glDisable (GL_ALPHA_TEST);

   Profiler::getInstance().end();
   // Profiler::getInstance().draw(*GuiManager::getInstance().getDefaultFont());

   SDL_GL_SwapBuffers ();
}

void Game::drawFrame(float delta)
{
}
