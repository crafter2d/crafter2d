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
#ifndef SIMULATION_FILLER_H_
#define SIMULATION_FILLER_H_

#include "../nodevisitor.h"

class Simulator;

class SimulationFiller : public NodeVisitor
{
public:
   static void add(Simulator& simulator, SceneObject& object);

   SimulationFiller(Simulator& simulator);
   ~SimulationFiller();

   virtual void   visitWorld(World* object);
   virtual void   visitObject(Object* object);

private:
   Simulator& mSimulator;
};

#endif
