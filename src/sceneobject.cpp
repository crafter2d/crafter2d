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
#include "sceneobject.h"
#ifndef JENGINE_INLINE
#  include "sceneobject.inl"
#endif

#include <tinyxml.h>

#include "dirtyset.h"
#include "game.h"
#include "gameconfiguration.h"
#include "nodevisitor.h"
#include "scenegraph.h"
#include "scopedtransform.h"

ABSTRACT_IMPLEMENT_REPLICATABLE(SceneObjectId, SceneObject, NetObject)

SceneObject::SceneObject():
   mName(),
   xmlfile(),
   children(),
   parent(NULL)
{
}

SceneObject::~SceneObject()
{
   ASSERT(parent == NULL);
}

/*!
    \fn SceneObject::create(const char* filename)
    \brief Constructs an object from an XML file.
    \param file File name of the object XML file
    \retval true the object is loaded and initialized without errors.
    \retval false there was an error during loading, consult the log file for a reason
 */
bool SceneObject::create(const char* file)
{
   if ( file != NULL )
   {
      xmlfile = file;
   }

   const std::string& objectpath = Game::getInstance().getConfiguration().getObjectPath();
   std::string path = objectpath + xmlfile;

   TiXmlDocument doc(path);
   return doc.LoadFile() && load(doc);
}

bool SceneObject::load (TiXmlDocument& doc)
{
   return false;
}

bool SceneObject::load(TiXmlElement& element)
{
   return true;
}

bool SceneObject::save(TiXmlElement& element)
{
   return true;
}

/// \fn SceneObject::destroy()
/// \brief Releases memory and children
///
/// This method is automatically called by the SceneGraph destructor. It releases memory of the name and
/// recursively calls destroy for the children of the current node.
void SceneObject::destroy ()
{
   if ( parent )
   {
      parent->remove(this);
      parent = NULL;
   }

   // remove the object from the scenegraph
   SceneGraph& graph = getSceneGraph();
   graph.removeObject(mName);

	// destroy the children
   removeAll();
}

/// \fn SceneObject::contains(const SceneObject& object) const
/// \returns True when object is a child of this instance. False otherwise.
bool SceneObject::contains(const SceneObject& object) const
{
  return std::find(children.begin(), children.end(), &object) != children.end();
}

/// \fn SceneObject::add(SceneObject* child)
/// \brief Adds a child node to this node and optionally supplies it a name
/// \param child Child to add to this node
void SceneObject::add(SceneObject* child)
{
   // copy the name
   child->parent = this;

   // add object to the scenegraph list
   SceneGraph& graph = getSceneGraph();
   graph.addObject(child);

   // add new child to the back of the list
   children.push_back (child);
}

SceneGraph& SceneObject::getSceneGraph()
{
   if ( isReplica() )
      return Game::getInstance().getClient().getSceneGraph();
   else
      return Game::getInstance().getServer().getSceneGraph();
}

void SceneObject::removeAll()
{
   while (children.size() > 0) {
      SceneObject* child = children.front();
      child->destroy();
      delete child;
   }
}

void SceneObject::remove(SceneObject* child)
{
   children.remove(child);
}

const SceneObject::SceneObjectList& SceneObject::getChildren() const
{
   return children;
}

void SceneObject::accept(NodeVisitor& nv)
{ 
   nv.visitSceneObject(this); 
}

const Vector& SceneObject::getPosition() const
{
   PURE_VIRTUAL
   return Vector::zero();
}

/// \fn SceneObject::setName(const char* newName)
/// \brief Sets the name of this object.
/// \param n the new name for this object
void SceneObject::setName(const std::string& name)
{
   if ( name != mName )
   {
      std::string previousName = mName;

      mName = name;

      getSceneGraph().notifyNameChanged(*this, previousName);
   }
}

/// \fn SceneObject::update(float delta)
/// \brief Update mechanism for the derived nodes of the SceneObject class.
/// \param tick current tick in the game (in msecs)
void SceneObject::update(DirtySet& dirtyset, float delta)
{
   doUpdate(delta);

   if ( isDirty() )
      dirtyset.reportDirty(*this);

   // update the children
   SceneObjectList::iterator it = children.begin();
   for (; it != children.end(); it++)
   {
      SceneObject& sceneobject = *(*it);
      sceneobject.setDirty(false);
      sceneobject.update(dirtyset, delta);
   }
}

void SceneObject::updateClient(float delta)
{
   doUpdateClient(delta);

   // update the children
   SceneObjectList::iterator it = children.begin();
   for (; it != children.end(); it++)
   {
      SceneObject& sceneobject = *(*it);
      sceneobject.setDirty(false);
		sceneobject.updateClient(delta);
   }
}

/// \fn SceneObject::doUpdate(DirtySet& dirtyset, float delta)
void SceneObject::doUpdate(float)
{
   PURE_VIRTUAL;
}

/// \fn void SceneObject::doUpdateClient(float)
void SceneObject::doUpdateClient(float)
{
   PURE_VIRTUAL;
}

/// \fn SceneObject::draw()
/// \brief Draw mechanism for the derived nodes of the SceneObject class.
void SceneObject::draw(bool traverse)
{
   ScopedTransform transform(getPosition());

   doDraw();

   if ( traverse )
   {
      // draw the children
	   SceneObjectList::iterator it = children.begin();
	   for ( ; it != children.end(); it++ )
      {
         SceneObject& sceneobject = *(*it);
		   sceneobject.draw();
      }
   }
}

/// \fn SceneObject::doDraw()
void SceneObject::doDraw()
{
   PURE_VIRTUAL;
}

/// \fn SceneObject::find(const char* node, bool recurse)
/// \brief Searches for a node with name 'node' and optionally recurses into the children.
/// \param node name of the node to find
/// \param recurse recurce into the children
SceneObject* SceneObject::find(const std::string& node, bool recurse)
{
   // see if this node is the one were looking for
   if ( mName == node )
      return this;

   if ( recurse )
   {
      // recursively search the children for the node
      SceneObjectList::iterator it = children.begin();
      for (; it != children.end(); it++)
      {
         SceneObject* n = (*it)->find(node, recurse);
         if ( n != NULL )
            return n;
      }
   }
   return NULL;
}

void SceneObject::pack(BitStream& stream) const
{
   stream << mName;
}

void SceneObject::unpack(BitStream& stream)
{
   stream >> mName;
}
