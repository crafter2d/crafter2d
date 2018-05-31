/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "contentloader.h"

#include "core/defines.h"

ContentLoader::ContentLoader(Process& process):
   mProcess(process),
   mpSimulator(nullptr)
{
}

// - Get/set

Process& ContentLoader::getProcess()
{
   return mProcess;
}

Simulator& ContentLoader::getSimulator()
{
   ASSERT_PTR(mpSimulator);
   return *mpSimulator;
}

void ContentLoader::setSimulator(Simulator& simulator)
{
   mpSimulator = &simulator;
}
