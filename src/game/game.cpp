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

#include "core/system/platform.h"
#include "core/system/timer.h"
#include "core/system/timerdelta.h"
#include "core/vfs/filesystem.h"
#include "core/smartptr/autoptr.h"

#include "engine/script/script.h"
#include "engine/tools/profiler/profiler.h"
#include "engine/tools/profiler/profilerinstance.h"
#include "engine/physics/physicsfactory.h"
#include "engine/physics/box2d/box2dfactory.h"
#include "engine/physics/simulationfactoryregistry.h"
#include "engine/net/netobjectfactory.h"
#include "engine/net/netconnection.h"
#include "engine/client.h"
#include "engine/server.h"

#include "console.h"

/*!
    \fn Game::Game()
	 \brief Initialized member variables
 */
Game::Game():
   mTitle(),
   mpWindowFactory(NULL),
   mpTimerData(NULL),
   mpClient(NULL),
   mpServer(NULL),
   mActive(false)
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
    \fn Game::create()
	 \brief Call this function to initialize the game engine and prepare it for running a game.
	 \return true if initialized successfull, false otherwise
 */

bool Game::create()
{
   Log& log = Log::getInstance();
   log << "JEngine SSE V0.4.8 - Copyright 2012 - Jeroen Broekhuizen\n";
   log << "Released under LGPL, see license.txt file for more info.\n";
   log << "---------------------------------------------------------\n";

   mpTimerData = TIMER.createData();

   FileSystem::getInstance().addPath("../scripts");
   FileSystem::getInstance().addPath("../images");
   
   // register the physics factory
   SimulationFactoryRegistry::getInstance().addFactory(new PhysicsFactory());
   SimulationFactoryRegistry::getInstance().addFactory(new Box2DFactory());

   log << "\n-- Initializing Graphics --\n\n";

   // initialize the console
   Console& console = Console::getInstance();
   console.create ();

#ifdef WIN32
   if ( !NetConnection::initialize() )
   {
      return false;
   }

   // initialize the AVI library
   AVIFileInit();
#endif

   NetObjectFactory::getInstance().initialize();

   // reload the contents of the log file for the console
   console.reload();

   log << "\n-- Running Game --\n\n";
   
   // give the game time to load in stuff before window shows up
   // (after that, the game has to keep track of it's own state)
   mActive = initGame();
   if ( !mActive )
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

   // release timer data
   TIMER.releaseData(mpTimerData);

#ifdef WIN32
   // release the avi library
   AVIFileExit();
#endif
}

//////////////////////////////////////////////////////////////////////////
// - operations
//////////////////////////////////////////////////////////////////////////

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
		runFrame();
	}
}

/*!
    \fn Game::initGame()
	 \brief This function will be called during the initialization phase of the game. The window and OpenGL are both
	 already initialized. Overload it to customize it to your needs.
	 \return Return false to quit the game, or true if initialization was succesfull
 */
bool Game::initGame()
{
   mpServer = new Server();
   mpServer->create("demo.GameServer");

   mpClient = new Client();
   mpClient->setWindowFactory(*mpWindowFactory);
   mpClient->create("demo.GameClient");

	return true;
}

/*!
    \fn Game::endGame()
	 \brief When the game is closed this function will be called to allow you to free any allocated resources
	 and shutdown properly.
	 \return Nothing
 */
void Game::endGame()
{
   mpClient->destroy();
   delete mpClient;

   mpServer->destroy();
   delete mpServer;
}

/*!
    \fn Game::runFrame()
	 \brief Called when next frame should be rendered. Overload it to render your own custom objects.
	 \returns Nothing
 */
void Game::runFrame()
{
   //Profiler::getInstance().begin();
   
   TimerDelta timerdelta(getTimerData());
   float delta = timerdelta.getDelta();

   static float start = 0;
   static unsigned int frame = 0;

   if ( !isActive() )
      return;

   mpServer->update(delta);

   mpClient->update(delta);
   mpClient->render(delta);

   frame++;
   start += timerdelta.getDelta();
   if ( start >= 1.0f )
   {
      std::cout << "Fps: " << frame << std::endl;
      start = 0;
      frame = 0;
   }

   // Profiler::getInstance().end();
   // Profiler::getInstance().draw(*GuiManager::getInstance().getDefaultFont());
}
