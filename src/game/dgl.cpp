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

/*!
 	\mainpage JEngine SSE - 2D Side Scoller Documentation
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
	<li>Lua scripting language
	<li>and much more.. :)
	</ul>
	
	\section lic_sec Licence
	The source code and documentation are released LGPL. This means that you may use, alter and redistribute the sources and documentation freely as long as the original licence notification is included both in open source as in commercial products. Credits are appreciated, but not required. If you have any questions, please contact me at <a href="mailto:jeroen@jeroen.homedns.org">jeroen@jeroen.homedns.org</a>.
	
	This software is provided 'as-is' without any express or implied warranty. In no event will
	the authors be held liable for any damages arising from the use of this software.
	
	JEngine makes use of:
	<ul>
	<li>Lua, http://www.lua.org/
	<li>toLua, http://www.tecgraf.puc-rio.br/~celes/tolua/
	<li>OpenAL, http://www.openal.org/
	<li>TinyXML, http://www.grinninglizard.com/tinyxml
	</ul>
 */

int main(int argc, char *argv[])
{
   AutoPtr<Game> game = new Game();
   if ( game->create() )
   {
      game->run ();
   }

   return EXIT_SUCCESS;
}