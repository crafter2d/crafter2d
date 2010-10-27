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

#include "dlgapp.h"

#include "gui/guimanager.h"
#include "gui/guifont.h"
#include "gui/guifocus.h"

#include "engine/scriptmanager.h"

IMPLEMENT_APPLICATION(MyGame)

MyGame::MyGame():
   Game()
{
}

MyGame::~MyGame()
{
}

bool MyGame::initGame()
{
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

	return Game::initGame();
}

void MyGame::endGame()
{
   // destroy the gui manager
	GuiManager::getInstance().destroy();
}

void MyGame::loadCustomScriptLibraries()
{
   Game::loadCustomScriptLibraries();
}

void MyGame::drawFrame(float delta)
{
   Game::drawFrame(delta);

   mCanvas.render(delta);   
}
