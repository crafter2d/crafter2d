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

#include <string>

#include "net/netobject.h"

#include "entities.h"
#include "idmanager.h"

class NodeVisitor;

class Entity : public NetObject
{
   enum { eNameDirty = 256 };

public:
   DEFINE_REPLICATABLE(Entity);

   Entity();
   virtual ~Entity() = 0;

 // get/set
   uint                    getId() const;
   void                    setId(uint id);

   bool                    hasParent() const;
   const Entity&           getParent() const;
         Entity&           getParent();
   void                    setParent(Entity& parent);

   bool                    hasFilename() const;
   const std::string&      getFilename() const;
   void                    setFilename(const std::string& filename);

   bool                    hasName() const;
   const std::string&      getName() const;
   void                    setName(const std::string& name);

   const std::string&      getClassName() const;
   void                    setClassName(const std::string& classname);

 // operations
   virtual void destroy();

 // update & drawing
   void update(float delta);
   void updateClient(float delta);
   void draw() const;

 // maintenance
   Entities& getChildren();
   void      add(Entity& entity);
   void      remove(Entity& entity);

 // visitor
   virtual void accept(NodeVisitor& visitor) = 0;

protected:

 // update & drawing
   virtual void doUpdate(float delta) = 0;
   virtual void doUpdateClient(float delta);
   virtual void doDraw() const = 0;

 // streaming
   virtual void doPack(DataStream& stream) const;
   virtual void doUnpack(DataStream& stream);

private:

   Id                mId;
   Entity*           mpParent;
   Entities          mChildren;
   std::string       mName;
   std::string       mXmlFile;
   std::string       mClassName;
};

#ifdef JENGINE_INLINE
#  include "entity.inl"
#endif

#endif // ENTITY_H
