/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _DLGAPP_H_
#define _DLGAPP_H_

#include "game.h"

/**
@author Jeroen Broekhuizen
\brief MyGame is the actual implementation of the Game. Game specific code should
be implemented in this class.

For example if a game specific tolua module must be loaded this class loadCustomScriptLibraries
function can be used. See its documentation about how to do that.
*/
class MyGame : public Game
{
public:
                  MyGame();
	virtual        ~MyGame();

   DECLARE_APPLICATION("MyGame")

protected:
	virtual bool   initGame();
   virtual void   loadCustomScriptLibraries();

	virtual void   drawFrame (Uint32 tick);
};

#endif
