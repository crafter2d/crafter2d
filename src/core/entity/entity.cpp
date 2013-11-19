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

#include "componentmessages/componentmessage.h"
#include "components/componentinterface.h"
#include "components/componentstructs.h"
#include "components/meshcomponent.h"

using namespace Graphics;

Entity::Entity():
   mId(-1),
   mComponents(*this),
   mpMeshComponent(NULL),
   mpController(NULL),
   mpParent(NULL),
   mChildren(),
   mTransform(),
   mOffset(),
   mName(),
   mXmlFile(),
   mClassName(),
   mDirtyFlag(false),
   mDirection(true)
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

void Entity::setTransform(const XForm& transform)
{
   mTransform = transform;

   ComponentMessage message(ComponentInterface::eUpdatedMsg); // called from the physics engine
   sendComponentMessage(message);
}

/// \fn Entity::setPosition(const Vector& p)
/// \brief Set the position of the object in world coordinates.
/// \param p the new position of the object
void Entity::setPosition(const Vector& p)
{
   mTransform.setPosition(p + mOffset);
   sendComponentMessage(ComponentMessage(ComponentInterface::ePositionChangedMsg));

   setDirty(ePositionDirty);

   //mChildren.setPosition(p);
}

/// \fn void setRotation(const float rotation)
/// \brief Sets the new degree of rotation of this object.
/// \param rotation the new rotation degree in radians
void Entity::setRotation(const float rotation) 
{
   mTransform.setAngle(rotation);
   sendComponentMessage(ComponentMessage(ComponentInterface::eRotationChangedMsg));

   setDirty(ePositionDirty);

   // mChildren.setRotation(deg);
}

/// \fn void setOffset(const Vector& offset)
/// \brief Updates the offset and directly the position of the entity as well!
void Entity::setOffset(const Vector& offset)
{
   mTransform.setPosition(mTransform.getPosition() + (offset - mOffset));
   mOffset = offset;

   sendComponentMessage(ComponentMessage(ComponentInterface::ePositionChangedMsg));
}

// - Operations

void Entity::initialize()
{
   Component* pcomponent = mComponents.findComponent(ComponentInterface::eMeshComponent);
   if ( pcomponent != NULL )
   {
      mpMeshComponent = static_cast<MeshComponent*>(pcomponent);
   }

   mChildren.initialize();
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

void Entity::flip()
{
   mDirection = !mDirection;
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

void Entity::add(Entity* pentity)
{
   pentity->setParent(*this);

   mChildren.add(pentity);
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

/*
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
      stream << mTransform.getPosition().x << mTransform.getPosition().y;
   }

   if ( isDirty(eRotationDirty) )
   {
      stream << mTransform.getAngle();
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
      stream >> pos.x >> pos.y;
      setPosition(pos);
   }

   if ( isDirty(eRotationDirty) )
   {
      float angle;
      stream >> angle;
      setRotation(angle);
   }
   
   mDirtyFlag = 0;
}
*/