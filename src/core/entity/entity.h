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

#include <cstdint>

#include "core/content/content.h"
#include "core/string/string.h"
#include "core/math/vector.h"
#include "core/math/xform.h"
#include "core/core_base.h"

#include "components/components.h"
#include "entities.h"
#include "idmanager.h"

namespace Graphics
{
   class Device;
   class RenderContext;
};

namespace c2d
{
   class Controller;
}

class NodeVisitor;

/// \brief Base class for all entities that can be shown in your game.

class CORE_API Entity : public IContent
{
public:
   enum { eNameDirty = 256, ePositionDirty = 2, eRotationDirty = 4 };

   Entity();
   virtual ~Entity();

 // get/set
   Id             getId() const { return mId; }
   void           setId(Id id) { mId = id; }

   bool           hasParent() const;
   const Entity&  getParent() const;
         Entity&  getParent();
   void           setParent(Entity& parent);
   
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

   c2d::Controller& getController();
   void             setController(c2d::Controller* pcontroller);
   
 // query
   bool hasLineOfSight(const Entity& that) const;

 // operations
   void initialize();
   void destroy();

   void addComponent(Component* pcomponent);

   void scale(float scale);
   void flip();

 // update & drawing
   void update(float delta);
   void draw(Graphics::RenderContext& context) const;

 // messaging
   template<typename Type>
   Type* getComponent(ComponentInterface::ComponentType type) {
      return mComponents.get<Type>(type);
   }
   void sendComponentMessage(ComponentMessage& message);

 // maintenance
   Entities& getChildren();
   void      add(Entity* pentity);
   void      detach(Entity& entity);

 // visitor
   virtual void accept(NodeVisitor& visitor);

private:
   
 // data
   Id                   mId;
   mutable Components   mComponents;
   c2d::Controller*     mpController;
   Entity*              mpParent;
   Entities             mChildren;
   XForm                mTransform;
   Vector               mOffset;
   String               mName;
   String               mClassName;
   int                  mDirtyFlag;
};

#ifdef JENGINE_INLINE
#  include "entity.inl"
#endif

#endif // ENTITY_H
