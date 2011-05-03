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
#include "console.h"
#ifndef JENGINE_INLINE
#  include "console.inl"
#endif

#include <fstream>

/*
#include "gui/guidialog/guidialog.h"
#include "gui/edit/guiedit.h"
#include "gui/guilistbox.h"
*/

#include "core/log/log.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "game.h"

Console::Console()
{
}

Console::~Console()
{
   // controls will be automatically be released.
   //lines = 0;
   //input = 0;
}

Console& Console::getInstance()
{
   static Console console;
   return console;
}

void Console::create()
{
   /*
	GuiRect rect(0, 450, 0, 300);
	GuiStyle style = GUI_NODESTROY | GUI_BORDER | GUI_BACKGROUND | GUI_VISIBLE;
   GuiWnd::create(0x03000, rect, "Console", style);

	rect.set(5,440,5,275);
   lines = new GuiListBox();
   lines->create(0x03001, rect, "Log", GUI_BORDER|GUI_BACKGROUND|GUI_VISIBLE, this);

	rect.set(5, 440, 280, 295);
   input = new GuiEditBox();
   input->create(0x03002, rect, "", GUI_BORDER | GUI_BACKGROUND|GUI_VISIBLE, this);
   */
}

void Console::reload()
{
   /*
   lines->removeAll();
   std::ifstream file("log.txt");
   while (!file.eof()) {
      char line[1024];
      file.getline(line, 1024);
      lines->addString(line);
   }
   */
}

void Console::printf (const char* msg, ...)
{
   //assert(lines && "Console.print: the console has not be created yet.");

   char text[256];
	va_list ap;
	if (msg == NULL)
		return;

	// format the input text
	va_start (ap, msg);
	vsprintf (text, msg, ap);
	va_end (ap);

   //lines->addString(text);
   Log::getInstance() << text << '\n';
}

void Console::error(const char* msg, ...)
{
   ASSERT_MSG(lines, "ERROR - the console has not been created yet.");
   ASSERT_PTR(msg);

   char text[256];
   va_list ap;

   va_start(ap, msg);
   vsprintf(text, msg, ap);
   va_end(ap);

   std::string errormsg = "ERROR - ";
   errormsg += text;

   //lines->addString(errormsg.c_str());
   Log::getInstance() << errormsg.c_str() << '\n';
}

void Console::warning(const char* msg, ...)
{
   ASSERT_MSG(lines, "ERROR - the console has not been created yet.");
   ASSERT_PTR(msg);

   char text[256];
   va_list ap;

   va_start(ap, msg);
   vsprintf(text, msg, ap);
   va_end(ap);

   std::string errormsg = "WARNING - ";
   errormsg += text;

   //lines->addString(errormsg.c_str());
   Log::getInstance() << errormsg.c_str() << '\n';
}

void Console::print(const char* msg)
{
   ASSERT_MSG(lines, "Console.print: the console has not be created yet.");

   //lines->addString(msg);
   Log::getInstance() << msg << '\n';
}

void Console::show()
{
   //Game::getInstance().getCanvas().pushWindow(this);
}

void Console::hide()
{
   //Game::getInstance().getCanvas().popWindow(this);
}

void Console::onKeyDown (int which, bool shift, bool ctrl, bool alt)
{
   if ( which == SDLK_RETURN )
   {
      // execute the command
      //ScriptManager::getInstance().executeLine(input->getCaption().c_str());
      //input->setCaption("");
   }
   else if (which == SDLK_BACKQUOTE)
   {
      // hide the console
      //Game& game = Game::getInstance();
      //game.getCanvas().endModal(0);
   }
}
