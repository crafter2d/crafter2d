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
#ifndef _CREATURE_H_
#define _CREATURE_H_

#include "object.h"

class BitStream;
class NodeVisitor;
class TiXmlDocument;

/**
@author Jeroen Broekhuizen
\brief Interface for all creatures.

Creatures can be implemented using the Creature class. It currently supports only the moveto
function, which makes this creature follow another creature.
*/
class Creature: public Object
{
public:
	                  Creature();
	virtual           ~Creature();

   virtual void      update(DirtySet& dirtyset, float delta);

	void              moveto(Creature* t);

 // visitor interface
   void              accept(NodeVisitor& nv);

 // serialization interface
   virtual void      pack(BitStream& stream);
   virtual void      unpack(BitStream& stream);

   DEFINE_REPLICATABLE(Creature)

protected:
   virtual bool      load(TiXmlDocument& doc);
   const Creature&   me();

private:
	Creature*       target;
};

#ifdef JENGINE_INLINE
#  include "creature.inl"
#endif

#endif
