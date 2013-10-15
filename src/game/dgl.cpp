/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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

#include "core/smartptr/autoptr.h"

#include "game.h"
#include "sdlgamewindowfactory.h"
#include "win32gamewindowfactory.h"

/*!
 	\mainpage Crafter 2D - Game framework for traditional platform games
 	\section intro_sec Introduction
 	These pages contain the complete documentation of the Crafter 2D sources. You will find here
	all information about the classes, structures, etc used to implement the engine.

	\section feat_sec Features
	Crafter 2D has the following features:
	<ul>
	<li>Object oriented design (C++)
	<li>DirectX and OpenGL
   <li>Full shader support (HLSL & GLSL)
	<li>Multiplatform: Windows and Linux
   <li>Supports single & multi player games
	<li>Scripting language
	<li>and much more..
	</ul>

	\section lic_sec Licence
	The source code and documentation are released LGPL. This means that you may use, alter and 
   redistribute the sources and documentation freely as long as the original licence notification
   is included both in open source as in commercial products. Credits are appreciated, but not
   required. If you have any questions, please contact me at
   <a href="mailto:jengine.sse@live.nl">jengine.sse@live.nl</a>.

	This software is provided 'as-is' without any express or implied warranty. In no event will
	the authors be held liable for any damages arising from the use of this software.

	Crafter 2D supports/uses the following libraries:
	<ul>
   <li>Antlr, http://antlr.org/
   <li>Box2D, http://box2d.org/
   <li>TinyXML, http://www.grinninglizard.com/tinyxml
   <li>Zlib, http://zlib.net/
	<li>FMOD, http://www.fmod.com/
   <li>DirectX, http://www.microsoft.com/directx
   <li>SDL, http://libsdl.org/
   <li>SOIL, http://www.lonesock.net/soil.html
   <li>GLee, http://elf-stone.com/glee.php
   <li>FreeType2, http://freetype.org/
   <li>CxxTest, http://cxxtest.tigris.org/
	</ul>
 */

int main(int argc, char *argv[])
{
   Win32GameWindowFactory windowfactory;
   //SDLGameWindowFactory windowfactory;

   AutoPtr<Game> game = new Game();
   game->setWindowFactory(windowfactory);
   if ( game->create() )
   {
      game->run ();
   }

   return 0;
}
