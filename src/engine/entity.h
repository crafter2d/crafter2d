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
#ifndef ENTITY_H
#define ENTITY_H

#include "core/string/string.h"
#include "core/math/vector.h"
#include "core/math/xform.h"

#include "net/netobject.h"
#include "components/components.h"

#include "entities.h"
#include "idmanager.h"

namespace Graphics
{
   class Device;
   class RenderContext;
};

class MeshComponent;
class NodeVisitor;

/// \brief Base class for all entities that can be shown in your game.

class Entity : public NetObject
{
   enum { eNameDirty = 256, ePositionDirty = 2, eRotationDirty = 4 };

public:
   DEFINE_REPLICATABLE(Entity);

   Entity();
   virtual ~Entity();

 // get/set
   uint           getId() const;
   void           setId(uint id);

   bool           hasParent() const;
   const Entity&  getParent() const;
         Entity&  getParent();
   void           setParent(Entity& parent);

   bool           hasFilename() const;
   const String&  getFilename() const;
   void           setFilename(const String& filename);

   bool           hasName() const;
   const String&  getName() const;
   void           setName(const String& name);

   const String&  getClassName() const;
   void           setClassName(const String& classname);
   
   bool           isDirty() const;
   bool           isDirty(int flag) const;
   void           setDirty(int flag);
   void           resetDirty();

   const XForm&   getTransform() const;
   void           setTransform(const XForm& transform);

   const Vector&  getPosition() const;
   void           setPosition(const Vector& p);

   float          getRotation() const;
   void           setRotation(float rot);

   const Vector&  getOffset() const;
   void           setOffset(const Vector& offset);

 // operations
   void initialize();
   void destroy();

   void addComponent(Component* pcomponent);

 // update & drawing
   void update(float delta);
   void updateClient(float delta);
   void draw(Graphics::RenderContext& context) const;

 // messaging
   void sendComponentMessage(ComponentMessage& message);

 // maintenance
   Entities& getChildren();
   void      add(Entity* pentity);
   void      remove(Entity& entity);

 // visitor
   virtual void accept(NodeVisitor& visitor);

protected:

 // get/set
   bool                 hasMesh() const;
   const MeshComponent& getMesh() const;
         MeshComponent& getMesh();

  // streaming
   virtual void doPack(DataStream& stream) const;
   virtual void doUnpack(DataStream& stream);

private:

   Id                mId;
   mutable Components mComponents;
   MeshComponent*    mpMeshComponent;
   Entity*           mpParent;
   Entities          mChildren;
   XForm             mTransform;
   Vector            mOffset;
   String            mName;
   String            mXmlFile;
   String            mClassName;
   unsigned int		 mDirtyFlag;
};

#ifdef JENGINE_INLINE
#  include "entity.inl"
#endif

#endif // ENTITY_H
