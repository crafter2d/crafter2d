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

#include <cstdlib>

#include "game.h"
#include "sdlgamewindowfactory.h"

/*!
 	\mainpage JEngine SE - 2D Simple Scroll Engine Documentation
 	\section intro_sec Introduction
 	These pages contain the complete documentation of the JEngine sources. You will find here
	all information about the classes, structures, etc used to implement the engine.

	\section feat_sec Features
	JEngine has the following features:
	<ul>
	<li>%Object oriented design (C++)
	<li>OpenGL support via SDL
	<li>Multiplatform: Windows and Linux
	<li>GLSL and CG shading
	<li>Scripting language
	<li>and much more.. :)
	</ul>

	\section lic_sec Licence
	The source code and documentation are released LGPL. This means that you may use, alter and redistribute the sources and documentation freely as long as the original licence notification is included both in open source as in commercial products. Credits are appreciated, but not required. If you have any questions, please contact me at <a href="mailto:jeroen@jeroen.homedns.org">jeroen@jeroen.homedns.org</a>.

	This software is provided 'as-is' without any express or implied warranty. In no event will
	the authors be held liable for any damages arising from the use of this software.

	JEngine supports/uses the following libraries:
	<ul>
   <li>Antlr, http://antlr.org/
   <li>Box2D, http://box2d.org/
   <li>CxxTest, http://cxxtest.tigris.org/
   <li>DevIL, http://openil.sourceforge.net/
   <li>FreeType2, http://freetype.org/
   <li>GLee, http://elf-stone.com/glee.php
   <li>ICU, http://site.icu-project.org/
   <li>NVidia Cg, http://developer.nvidia.com/
   <li>Ogg/Vorbis, http://xiph.org/
	<li>OpenAL, http://www.openal.org/
   <li>SDL, http://libsdl.org/
   <li>SOIL, http://www.lonesock.net/soil.html
	<li>TinyXML, http://www.grinninglizard.com/tinyxml
   <li>Zlib, http://zlib.net/
	</ul>
 */

int main(int argc, char *argv[])
{
   SDLGameWindowFactory windowfactory;

   AutoPtr<Game> game = new Game();
   game->setWindowFactory(windowfactory);
   if ( game->create() )
   {
      game->run ();
   }

   return EXIT_SUCCESS;
}
