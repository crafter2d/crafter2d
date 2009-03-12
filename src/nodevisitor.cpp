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
#include "nodevisitor.h"
#ifndef JENGINE_INLINE
#  include "nodevisitor.inl"
#endif

#include "creature.h"
#include "sceneobject.h"

NodeVisitor::NodeVisitor()
{
}

NodeVisitor::~NodeVisitor()
{
}

void NodeVisitor::traverse(SceneObject& node)
{
   // let the specialized traverser handle this node
   node.accept(*this);

   // traverse the children of this node
   const SceneObject::SceneObjectList& children = node.getChildren();
   SceneObject::SceneObjectList::const_iterator it = children.begin();
   for ( ; it != children.end(); ++it )
   {
      SceneObject* pobject = (*it);
      traverse(*pobject);
   }
}
