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

#include "gui/guimanager.h"
#include "gui/guidesigner.h"
#include "gui/guifocus.h"
#include "gui/guidialog.h"
#include "gui/guifont.h"

#include "net/netobjectfactory.h"

#include "console.h"
#include "opengl.h"
#include "scriptmanager.h"
#include "sound.h"
#include "object3d.h"
#include "creature.h"
#include "gameconfiguration.h"
#include "gameconfigurationselector.h"
#include "gamesettings.h"

int main(int argc, char *argv[])
{
   Game& game = Game::getInstance();
   game.create();
   game.run ();
   game.destroy ();

   return EXIT_SUCCESS;
}

/*!
    \fn Game::Game()
	 \brief Initialized member variables
 */
Game::Game():
   active(true),
   keyboardEventsEnabled(true),
   width(800),
   height(600),
   canvas(),
   server(),
   client(),
   mSettings(),
   mGlClearColor(),
   mTitle(),
   mpConfiguration(NULL),
   window(NULL),
   mpDesigner(NULL),
   bitdepth(32),
   videoFlags(0),
   mDesigning(false)
{
}

/*!
    \fn Game::~Game()
	 \brief Calls destroy when game is finished.
 */
Game::~Game()
{
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
   log << "JEngine SSE V0.4.4 - Copyright 2006 - Jeroen Broekhuizen\n";
   log << "Released under LGPL, see license.txt file for more info.\n";
   log << "---------------------------------------------------------\n";

   // initialize the video library
   if (SDL_Init (SDL_INIT_VIDEO) < 0) {
      log << "Couldn't initialize the SDL library!";
      return false;
   }

   // initialize the Lua scripting environment
   ScriptManager& scriptMgr = ScriptManager::getInstance ();
   scriptMgr.initialize ();
   loadCustomScriptLibraries();

   scriptMgr.setObject(&canvas, "GuiCanvas", "canvas");
   scriptMgr.setObject(&server, "Server", "server");
   scriptMgr.setObject(&client, "Client", "client");
   scriptMgr.setObject(&Console::getInstance(), "Console", "console");
   scriptMgr.setObject(&SoundManager::getInstance(), "SoundManager", "SoundSystem");
   scriptMgr.setObject(&scriptMgr, "ScriptManager", "scriptmgr");
   scriptMgr.setObject(&GuiManager::getInstance(), "GuiManager", "guimanager");
   scriptMgr.setObject(&GuiFocus::getInstance(), "GuiFocus", "focus");

   // load the engine settings
   mSettings.initialize();

   log << "\n-- Initializing Graphics --\n\n";

   int videoFlags = preConfigScreen();
   if (videoFlags == 0)
      return false;

   // set up opengl viewport & matrices
   if ( !onResize(mSettings.getWidth(), mSettings.getHeight()) )
      return false;

   // set the caption of the window
   SDL_WM_SetCaption(mTitle.c_str(), mTitle.c_str());
   SDL_EnableUNICODE(1);

   // now initialize OpenGL for rendering
   initOpenGL ();

   // initialize the window manager
   GuiManager& manager = GuiManager::getInstance();
   manager.initialize();

   GuiFont* font = new GuiFont ();
   font->initialize ("amersn.ttf", 8);

   manager.setDefaultFont (font);
   manager.setDefaultTextColor(mSettings.getTextColor());

   // initialize the console
   Console& console = Console::getInstance();
   console.create ();

   // create the gui canvas
   canvas.create(0, GuiRect(0, mSettings.getWidth(), 0, mSettings.getHeight()));
   canvas.changeDefaultColor(GuiCanvas::GuiWindowColor, mSettings.getWindowColor());
   canvas.changeDefaultColor(GuiCanvas::GuiBorderColor, mSettings.getBorderColor());

#ifdef WIN32
   NetConnection::initialize();

   // initialize the AVI library
   AVIFileInit();
#endif

   NetObjectFactory::getInstance().initialize();

   log << "\n-- Initializing Sound --\n\n";

   // initialize the sound system
   SoundManager::getInstance().initialize();

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

   scriptMgr.executeScript("main.lua");

   return true;
}

/*!
    \fn Game::destroy()
	 \brief When finished this function will be called automatically by the destructor.
	 \return Nothing
 */
void Game::destroy()
{
   server.shutdown();

   // free the game resources
	endGame ();

	// release the Lua scripting environment
	ScriptManager::getInstance().destroy ();

	// release the sound system
	SoundManager::getInstance().destroy ();

	// destroy the gui manager
	GuiManager::getInstance().destroy();

#ifdef WIN32
   // release the avi library
   AVIFileExit();
#endif

	if (window) {
		// release main window
		SDL_FreeSurface (window);
		window = NULL;
	}

	// finish of SDL
	SDL_Quit ();
}

/// \fn Game::loadCustomScriptLibraries()
/// \brief Called by the game framework when custom script libraries should be loaded.
void Game::loadCustomScriptLibraries()
{
}

int Game::preConfigScreen()
{
   Log& log = Console::getLog();

	const SDL_VideoInfo *videoInfo;
	videoInfo = SDL_GetVideoInfo();
	if (videoInfo == NULL)
   {
      log << "Can't get video information about graphics card.";
		return 0;
	}

	// set up initial flags
	videoFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
   if ( mSettings.getFullScreen() )
      videoFlags |= SDL_FULLSCREEN;

	// try to use hardware surface (in videomemory)
	if (videoInfo->hw_available)
   {
		videoFlags |= SDL_HWSURFACE;
      log << "Hardware rendering is supported\n";
	}
	else
   {
      log << "Switching to software rendering mode\n";
		videoFlags |= SDL_SWSURFACE;
	}

	// see if hardware blitting is supported on the platform
	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

   int width    = mSettings.getWidth();
   int height   = mSettings.getHeight();
   int bitdepth = mSettings.getBitDepth();

	// make sure that the bitdepth is valid
   bitdepth = SDL_VideoModeOK(width, height, bitdepth, videoFlags);
	if (bitdepth == 0)
   {
      log << "Bitdepth is not supported with resolution " << width << "x" << height << "\n";
		return 0;
	}
	else
      log << "Screen resolution:\t" << width << "x" << height << "x" << bitdepth << "\n";

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	return videoFlags;
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
void Game::initOpenGL()
{
   const GuiColor& clearcolor = mSettings.getClearColor();
   glClearColor(clearcolor.r, clearcolor.g, clearcolor.b, 0.0f);
	glClearDepth(1.0f);

	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel (GL_SMOOTH);

	OpenGL::initialize ();
}

/*!
    \fn Game::run()
	 \brief Runs the main event loop of the game.
	 \return Nothing
 */
void Game::run()
{
	while ( active )
   {
		processFrame();
	}
}

void Game::processFrame()
{
   SDL_Event event;

	while (SDL_PollEvent (&event))
   {
		switch (event.type) {
		case SDL_VIDEORESIZE:
  		   if (!onResize (event.resize.w, event.resize.h))
				active = false;
			break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
         onKeyboardEvent(event.key);
         break;
      case SDL_MOUSEMOTION:
         onMouseMoveEvent(event.motion);
         break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
         onMouseButtonEvent (event.button);
         break;
		case SDL_QUIT:
			active = false;
			break;
		}
	}

	// get the keystate
	Uint8* keys = SDL_GetKeyState (NULL);

	// do a frame
	//updateFrame (keys);
	runFrame ();
}

void Game::onKeyboardEvent (const SDL_KeyboardEvent& event)
{
   SDLMod mode = SDL_GetModState();

   bool shift = IS_SET(mode, KMOD_SHIFT);
   bool ctrl  = IS_SET(mode, KMOD_CTRL);
   bool alt   = IS_SET(mode, KMOD_ALT);

   if (event.type == SDL_KEYDOWN)
   {
      canvas.onKeyDown (event.keysym.unicode == 0 ? event.keysym.sym : event.keysym.unicode & 0x7F, shift, ctrl ,alt);
   }
   else
   {
      switch ( event.keysym.sym )
      {
      case SDLK_F3:
         switchDesigner();
         break;
      case SDLK_F4:
         switchEditor();
         break;
      default:
         canvas.onKeyUp (event.keysym.unicode == 0 ? event.keysym.sym : event.keysym.unicode & 0x7F);
         break;
      }
   }
}

void Game::onMouseButtonEvent (const SDL_MouseButtonEvent& event)
{
   int flags = mouseKeyFlags();

   switch (event.type) {
      case SDL_MOUSEBUTTONUP:
         if (event.button == SDL_BUTTON_LEFT)
            canvas.onLButtonUp(GuiPoint(event.x, event.y), flags);
         else if (event.button == SDL_BUTTON_WHEELUP)
            canvas.onMouseWheel(GuiPoint(event.x, event.y), -1, flags);
         else if (event.button == SDL_BUTTON_WHEELDOWN)
            canvas.onMouseWheel(GuiPoint(event.x, event.y), 1, flags);
         break;
      case SDL_MOUSEBUTTONDOWN:
         if (event.button == SDL_BUTTON_LEFT)
            canvas.onLButtonDown(GuiPoint(event.x, event.y), flags);
         else if (event.button == SDL_BUTTON_RIGHT)
            canvas.onRButtonDown(GuiPoint(event.x, event.y), flags);
         break;
   }
}

void Game::onMouseMoveEvent (const SDL_MouseMotionEvent& event)
{
   int flag = mouseKeyFlags();
   if (event.state == SDL_BUTTON(1))
      flag |= GuiLButton;
   canvas.onMouseMove (GuiPoint(event.x, event.y), GuiPoint(event.xrel, event.yrel), flag);
}

int Game::mouseKeyFlags()
{
   Uint8* keys;
   keys = SDL_GetKeyState (NULL);

   int flags = 0;
   if ( keys[SDLK_LCTRL] || keys[SDLK_RCTRL] )
   {
      flags = GuiCtrl;
   }
   if ( keys[SDLK_LALT] || keys[SDLK_RALT] )
   {
      flags |= GuiAlt;
   }
   if ( keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT] )
   {
      flags |= GuiShift;
   }

   return flags;
}

/*!
    \fn Game::onResize(int width, int height)
	 Called when the windows size has changed.
	 \param[in] width new width of the window
	 \param[in] height new height of the window
	 \return Nothing
 */
bool Game::onResize(int width, int height)
{
   // try to set the new video mode
   window = SDL_SetVideoMode (width, height, bitdepth, videoFlags);
   if ( window == NULL )
   {
      Console::getLog() << "Could not resize window to " << width << "x" << height << ": " << SDL_GetError();
      return false;
   }

   // set the new opengl states
   glViewport (0, 0, width, height);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (0,width,height,0,0,1000);

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
   return true;
}

/*!
   \fn Game::getWindowDimensions(int& w, int& h)
   \brief Returns the current dimensions of the window.
   \param w The width of the window
   \param h The height of the window
*/
void Game::getWindowDimensions(int& w, int& h)
{
   w = width;
   h = height;
}

/*!
    \fn Game::initGame()
	 \brief This function will be called during the initialization phase of the game. The window and OpenGL are both
	 already initialized. Overload it to customize it to your needs.
	 \return Return false to quit the game, or true if initialization was succesfull
 */
bool Game::initGame()
{
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

	return active;
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
    \fn Game::updateFrame()
	 \brief Called when next frame should be prepared. Overload it to handle key actions etc before next
	 frame is rendered.
	 \returns Nothing
 */
void Game::updateFrame(Uint8* keys)
{
   if (keys[SDLK_F1])
      //SDL_WM_ToggleFullScreen (window);

   if (keys[SDLK_PRINT])
      SDL_SaveBMP (window, "screen1");
}

/*!
    \fn Game::runFrame()
	 \brief Called when next frame should be rendered. Overload it to render your own custom objects.
	 \returns Nothing
 */
void Game::runFrame()
{
   Uint32 tick = SDL_GetTicks ();

   ScriptManager::getInstance().update(tick);
   if ( !isActive() )
      return;

   server.update(tick);
   client.update(tick);

   // here also nothing happens (should be overloaded)
   glLoadIdentity ();
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glAlphaFunc (GL_GREATER, 0.1f);
   glEnable (GL_ALPHA_TEST);

   // call overloaded function
   canvas.render (tick);
   drawFrame (tick);

   //glDisable(GL_MULTISAMPLE);
   glDisable (GL_ALPHA_TEST);
   SDL_GL_SwapBuffers ();
}

void Game::switchDesigner()
{
   if ( !designing() )
   {
      designing(true);

      mpDesigner = new GuiDesigner();
      mpDesigner->create(1, GuiRect(0,300,0,300), "Designer", 1420, &canvas);
      mpDesigner->center();

      canvas.pushWindow(mpDesigner);
   }
   else if ( designing() )
   {
      designing(false);
      canvas.popWindow(mpDesigner);

      mpDesigner->destroy();
      delete mpDesigner;
      mpDesigner = NULL;
   }
}

void Game::switchEditor()
{
   GuiDialog* peditor = GuiManager::getInstance().loadDialogFromXML("te_editor");
   peditor->center();

   canvas.pushWindow(peditor);
}
