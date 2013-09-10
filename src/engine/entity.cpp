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

#include "core/graphics/device.h"
#include "core/defines.h"

#include "engine/components/componentinterface.h"
#include "engine/components/componentmessage.h"
#include "engine/components/componentstructs.h"
#include "engine/components/meshcomponent.h"
#include "engine/net/netstream.h"

using namespace Graphics;

ABSTRACT_IMPLEMENT_REPLICATABLE(EntityId, Entity, NetObject)

Entity::Entity():
   NetObject(),
   mId(-1),
   mComponents(*this),
   mpMeshComponent(NULL),
   mpParent(NULL),
   mChildren(),
   mName(),
   mXmlFile(),
   mClassName(),
   mDirtyFlag(false)
{
   if ( ((int)mId) == -1 )
   {
      mId = IdManager::getInstance().getNextId();
   }
}

Entity::~Entity()
{
   destroy();
}

// - Get/set

/// \fn SceneObject::setName(const char* newName)
/// \brief Sets the name of this object.
/// \param n the new name for this object
void Entity::setName(const String& name)
{
   if ( name != mName )
   {
      mName = name;

      setDirty(eNameDirty);
   }
}

bool Entity::hasMesh() const
{
   return mpMeshComponent != NULL;
}

const MeshComponent& Entity::getMesh() const
{
   ASSERT_PTR(mpMeshComponent);
   return *mpMeshComponent;
}

MeshComponent& Entity::getMesh()
{
   ASSERT_PTR(mpMeshComponent);
   return *mpMeshComponent;
}

// - Operations

void Entity::initialize()
{
   Component* pcomponent = mComponents.findComponent(ComponentInterface::eMeshComponent);
   if ( pcomponent != NULL )
   {
      mpMeshComponent = static_cast<MeshComponent*>(pcomponent);
   }
}

void Entity::destroy()
{
   if ( mpParent != NULL )
   {
      mpParent->remove(*this);
      mpParent = NULL;
   }
}

void Entity::addComponent(Component* pcomponent)
{
   mComponents.addComponent(pcomponent);
}

void Entity::update(float delta)
{
   mComponents.postMessage(ComponentMessage(ComponentInterface::eUpdateMsg, &delta));

   mChildren.update(delta);
}

void Entity::draw(Graphics::RenderContext& context) const
{
   if ( hasMesh() )
   {
      getMesh().render(context);
   }

   mChildren.draw(context);
}

// - Messaging
   
void Entity::sendComponentMessage(ComponentMessage& message)
{
   mComponents.postMessage(message);
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

void Entity::doPack(DataStream& stream) const
{
   NetObject::doPack(stream);

   stream.writeUint(mId);
   stream.writeUint(mDirtyFlag);

   if ( isDirty(eNameDirty) )
   {
      stream << getName();
   }

   if ( isDirty(ePositionDirty) )
   {
      PositionInfo info;
      const_cast<Entity&>(*this).sendComponentMessage(ComponentMessage(ComponentInterface::eQueryPositionMsg, &info));

      stream << info.transform.getPosition().x << info.transform.getPosition().y << info.transform.getAngle();
   }
}

void Entity::doUnpack(DataStream& stream)
{
   NetObject::doUnpack(stream);

   stream.readUint(mId);
   stream.readUint(mDirtyFlag);

   if ( isDirty(eNameDirty) )
   {
      stream >> mName;
   }

   if ( isDirty(ePositionDirty) )
   {
      Vector pos;
      float angle;

      stream >> pos.x >> pos.y >> angle;

      PositionInfo info;
      info.transform.set(pos, angle);
      sendComponentMessage(ComponentMessage(ComponentInterface::ePositionMsg, &info));
   }
   
   mDirtyFlag = 0;
}
