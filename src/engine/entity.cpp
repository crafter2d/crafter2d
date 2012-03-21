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
#include "entity.h"
#ifndef JENGINE_INLINE
#  include "entity.inl"
#endif

#include "core/defines.h"

#include "scopedtransform.h"

ABSTRACT_IMPLEMENT_REPLICATABLE(EntityId, Entity, NetObject)

Entity::Entity():
   NetObject(),
   mId(-1),
   mpParent(NULL),
   mChildren(),
   mName(),
   mXmlFile()
{
   if ( ((int)mId) == -1 )
      mId = IdManager::getInstance().getNextId();
}

Entity::~Entity()
{
   destroy();
}

// - Get/set

/// \fn SceneObject::setName(const char* newName)
/// \brief Sets the name of this object.
/// \param n the new name for this object
void Entity::setName(const std::string& name)
{
   if ( name != mName )
   {
      mName = name;

      setDirty(eNameDirty);
   }
}

// - Operations

void Entity::destroy()
{
   if ( mpParent != NULL )
   {
      mpParent->remove(*this);
      mpParent = NULL;
   }
}

void Entity::update(float delta)
{
   resetDirty();

   doUpdate(delta);

   mChildren.update(delta);
}

void Entity::updateClient(float delta)
{
   doUpdateClient(delta);
}

void Entity::draw() const
{
   doDraw();

   mChildren.draw();
}

void Entity::doUpdate(float delta)
{
   PURE_VIRTUAL
}

void Entity::doUpdateClient(float delta)
{
}

void Entity::doDraw() const
{
   PURE_VIRTUAL
}

// - Maintenance

void Entity::add(Entity& entity)
{
   entity.setParent(*this);

   mChildren.add(entity);
}

void Entity::remove(Entity& entity)
{
   mChildren.remove(entity);
}

// - Visitor

void Entity::accept(NodeVisitor& visitor)
{
   PURE_VIRTUAL;
}

// - Streaming

void Entity::doPack(BitStream& stream) const
{
   NetObject::doPack(stream);

   stream << mId;

   if ( isDirty(eNameDirty) )
   {
      stream << getName();
   }
}

void Entity::doUnpack(BitStream& stream, int dirtyflag)
{
   NetObject::doUnpack(stream, dirtyflag);

   stream >> mId;

   if ( IS_SET(dirtyflag, eNameDirty) )
   {
      stream >> mName;
   }
}
