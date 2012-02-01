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
#ifndef CONTENT_LOADER_H
#define CONTENT_LOADER_H

class Process;
class Simulator;

class ContentLoader
{
public:
   ContentLoader(Process& process): mProcess(process) {}

   Simulator& getSimulator() { return *mpSimulator; }
   void       setSimulator(Simulator& simulator) { mpSimulator = &simulator; }

protected:
   Process& getProcess() { return mProcess; }

private:
   Simulator* mpSimulator;
   Process& mProcess;
};

#endif // CONTENT_LOADER_H
