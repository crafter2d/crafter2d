/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

class NodeVisitor;
class SceneGraph;
class TiXmlDocument;
class TiXmlElement;

#define MAX_NAME_LEN    64

/**
@author Jeroen Broekhuizen

@section sceneobject_intro Using the SceneObject class

All objects which will be added to the scenegraph must be derived from the
SceneObject class. 
*/
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

	void                    add (SceneObject* child, const char* name=NULL);
   void                    remove(SceneObject* child);
   void                    removeAll();

   SceneObject*            find(const char* node, bool recurse = true);
	virtual void            update(Uint32 tick);
   virtual void            draw();

   virtual void            accept(NodeVisitor& nv);

   void                    setParent(SceneObject* p);
   void                    setName(const char* name);
   void                    setFilename(const std::string& filename);

   const char*             getName() const;
   const std::string&      getFilename() const;
   const SceneObjectList&  getChildren() const;
   SceneObject*            getParent();

   virtual void            pack(BitStream& stream) const;
   virtual void            unpack(BitStream& stream);
   
protected:
   virtual bool            load(TiXmlDocument& doc);

   char name[MAX_NAME_LEN];
   std::string       xmlfile;
   SceneObjectList   children;
   SceneObject*      parent;

private:
   static SceneGraph&      getSceneGraph(SceneObject* obj);
};

#ifdef JENGINE_INLINE
#  include "sceneobject.inl"
#endif

#endif
