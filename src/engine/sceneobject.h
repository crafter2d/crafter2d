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
#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

#include <SDL/SDL.h>
#include <list>

#include "net/netobject.h"

#include "idmanager.h"

class Body;
class DirtySet;
class NodeVisitor;
class SceneGraph;
class TiXmlDocument;
class TiXmlElement;

#define MAX_NAME_LEN    64

/// @section sceneobject_intro Using the SceneObject class
///
/// All objects which will be added to the scenegraph must be derived from the
/// SceneObject class. 

class SceneObject: public NetObject
{
public:
   typedef std::list<SceneObject*> SceneObjectList;

   DEFINE_REPLICATABLE(SceneObject)

                           SceneObject();
   virtual                 ~SceneObject();

   virtual bool            create(const char* filename=NULL);
   virtual void            destroy ();

   virtual bool            load(TiXmlElement& element);
   virtual bool            save(TiXmlElement& element);

   bool                    contains(const SceneObject& child) const;
   void                    add(SceneObject* child);
   void                    remove(SceneObject* child);
   void                    removeAll();

   SceneObject*            find(const Id& id, bool recurse = true);
   void                    update(DirtySet& dirtyset, float delta);
   void                    updateClient(float delta);
   void                    draw(bool traverse = true);

   virtual void            accept(NodeVisitor& nv);

   uint                    getId() const;

   bool                    hasParent() const;
   const SceneObject&      getParent() const;
   SceneObject&            getParent();
   void                    setParent(SceneObject* p);

   bool                    hasFilename() const;
   const std::string&      getFilename() const;
   void                    setFilename(const std::string& filename);

   bool                    hasName() const;
   const std::string&      getName() const;
   void                    setName(const std::string& name);

   const SceneObjectList&  getChildren() const;

   virtual SceneGraph&     getSceneGraph();

   virtual const Vector&   getPosition() const = 0;

   virtual void            pack(BitStream& stream) const;
   virtual void            unpack(BitStream& stream);
   
protected:
   friend class NewObjectEvent;

   virtual bool            load(TiXmlDocument& doc);

   virtual void            doUpdate(float delta) = 0;
   virtual void            doUpdateClient(float delta) = 0;
   virtual void            doDraw() = 0;

private:
   void setId(Id id);

   Id                mId;
   std::string       mName;
   std::string       mXmlFile;
   SceneObjectList   children;
   SceneObject*      parent;
};

#ifdef JENGINE_INLINE
#  include "sceneobject.inl"
#endif

#endif
