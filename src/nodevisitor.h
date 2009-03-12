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
#ifndef _NODEVISITOR_H_
#define _NODEVISITOR_H_

class SceneObject;
class World;
class Object;
class AnimObject;
class Creature;

/// Use a scene traverser to walk through the scenegraph and perform
/// some operation on it. The specialized traverser should overload the
/// doNode function to perform an operation on the node.
class NodeVisitor
{
public:
                  NodeVisitor();
   virtual        ~NodeVisitor();

           void   traverse(SceneObject& node);

   virtual void   visitSceneObject(SceneObject* object);
   virtual void   visitWorld(World* object);
   virtual void   visitObject(Object* object);
   virtual void   visitAnimObject(AnimObject* object);
   virtual void   visitCreature(Creature* object);
};

#ifdef JENGINE_INLINE
#  include "nodevisitor.inl"
#endif

#endif
