/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef SIMULATION_FACTORY_H
#define SIMULATION_FACTORY_H

class Simulator;
class Body;

class SimulationFactory
{
public:
   SimulationFactory();
   virtual ~SimulationFactory();

   /// \fn SimulationFactory::getName()
   /// Return the name of this factory. Used by registry to construct a derived factory.
   virtual const std::string& getName() const = 0;

   /// \fn SimulationFactory::createSimulator()
   /// Creates the simulator object used by the engine for updating the objects position and rotation.
   virtual Simulator* createSimulator() = 0;

   /// \fn SimulationFactory::createBody()
   /// Creates the body that matches the simulator constructed by this factory.
   virtual Body* createBody() = 0;
};

#endif
