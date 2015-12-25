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
#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine_base.h"

namespace c2d
{
   class Client;
   class Server;
}

class CommandLine;
class GameWindowFactory;
class TimerData;

/**
@author Jeroen Broekhuizen
\brief Actual game class responsible for launching and shutting down global systems.
*/
class ENGINE_API Game
{
public:
   explicit Game(CommandLine& cmdline);
   ~Game();

 // get/set interface
   GameWindowFactory&   getWindowFactory();
   void                 setWindowFactory(GameWindowFactory& windowfactory);

 // operations
   bool                 create();
   void                 destroy();
   void                 run();

private:
 // get/set
   TimerData&           getTimerData();

 // operations
   bool                 initGame();
   void                 endGame();
   void                 runFrame();

   CommandLine&         mCommandLine;
   GameWindowFactory*   mpWindowFactory;
   TimerData*           mpTimerData;
   c2d::Client*         mpClient;
   c2d::Server*         mpServer;
};

#ifdef JENGINE_INLINE
#  include "game.inl"
#endif

#endif
