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
#include "particlesystem.h"
#ifndef JENGINE_INLINE
#  include "particlesystem.inl"
#endif

#include <GL/GLee.h>
#include <GL/glu.h>
#include <tinyxml.h>

#include "codepath.h"
#include "log.h"
#include "object.h"
#include "opengl.h"
#include "script.h"
#include "scriptmanager.h"
#include "vertexbuffer.h"
#include "effect.h"
#include "process.h"

struct ParticleVertex {
	Vector pos;
	Color diffuse;
	Vector tex;
	Vector offset;
};

IMPLEMENT_REPLICATABLE(ParticleSystemId, ParticleSystem, Object)

/// \fn ParticleSystem::ParticleSystem()
/// \brief Initializes member variables.
ParticleSystem::ParticleSystem():
	activeList(0),
   freeList(0),
   maxBufferSize(0),
   emitRate(0),
   emitCount(0),
   active(0),
   maxActive(70),
	lastUpdate(0),
   lastInit(0),
   updateScript(0),
   buffer(0),
   path(0)
{
}

ParticleSystem::~ParticleSystem()
{
   destroy();
}

bool ParticleSystem::load(TiXmlDocument& doc)
{
   Log& log = Log::getInstance();

   // find particle information
   TiXmlElement* psystem = (TiXmlElement*)doc.FirstChild ("particlesystem");
	if ( psystem == NULL )
   {
      log.error("ParticleSystem.load: Invalid XML file format, particle system expected.\n");
		return false;
   }

   // load information
   setName(psystem->Attribute("name"));
   if ( psystem->QueryIntAttribute ("emitrate", &emitRate) != TIXML_SUCCESS ||
		  psystem->QueryIntAttribute ("emitcount", &emitCount) != TIXML_SUCCESS)
   {
      log.error("ParticleSystem.load: object needs to have emitrate and count.\n");
		return false;
	}

   // load texture data
	TiXmlElement* pelement = (TiXmlElement*)psystem->FirstChild ("texture");
	if ( pelement == NULL )
   {
		log.error("ParticleSystem.load: object has no texture.\n");
		return false;
	}
	else
   {
		TiXmlText* value = (TiXmlText*)pelement->FirstChild();
      texture = ResourceManager::getInstance().loadTexture(value->Value());
      if ( !(texture) )
         return false;
	}

   // load the update script
   pelement = (TiXmlElement*)psystem->FirstChild("update_script");
   if ( pelement != NULL )
   {
      TiXmlText* value = (TiXmlText*)pelement->FirstChild();
      ScriptManager& mgr = getSceneGraph().getProcess().getScriptManager();
	   updateScript = mgr.createScript();
      if ( !updateScript->load(value->Value()) )
         return false;
   }

   return prepare();
}

/// \fn ParticleSystem::prepare()
/// \brief Creates and initializes the particle system components neccessary for rendering.
bool ParticleSystem::prepare()
{
	//maxBufferSize = (size-1) * 2;
   maxBufferSize = 256;

	// create & initialize the code path
   path = OpenGL::createCodePath(CodePath::ECG);
	path->load("../shaders/pointsprite.cg", "");

   Effect effect;
   effect.setPath(path);

   int usage  = VertexBuffer::eStream | VertexBuffer::eWriteOnly;
   int format = VertexBuffer::eXY | VertexBuffer::eDiffuse | VertexBuffer::eTex0 | VertexBuffer::eTex1;

	// generate the vertex buffer
	buffer = OpenGL::createVertexBuffer ();
	if (!buffer->create(effect, maxBufferSize*4, usage, format))
		return false;

	srand (SDL_GetTicks ());
	return true;
}

/// \fn ParticleSystem::destroy()
/// \brief Release all dynamic objects inside this particle system.
void ParticleSystem::destroy()
{
   Object::destroy();

   if (path != 0) {
      delete path;
      path = 0;
   }
   if (buffer != 0) {
      delete buffer;
      buffer = 0;
   }
   if (updateScript != 0) {
      delete updateScript;
      updateScript = 0;
   }

   // release the particle lists of this particle system
   while (activeList != 0) {
      Particle* temp = activeList;
      activeList = activeList->next;
      delete temp;
   }
   while (freeList != 0) {
      Particle* temp = freeList;
      freeList = freeList->next;
      delete temp;
   }
}

/// \fn ParticleSystem::doUpdate (DirtySet& dirtyset, float delta)
/// \brief Fires the particle update script for every particle. After that the system
/// checks if there are still enough particles alive. If not, new particles are initialized
/// and put in the active list.
void ParticleSystem::doUpdate(DirtySet& dirtyset, float delta)
{
   if ( isReplica() )
   {
	   if ( delta > 100 )
      {
		   Particle** part = &activeList;
		   while ( *part != NULL)
         {
			   Particle *curpart = *part;
			   float lifetime = curpart->initTime;

			   if (lifetime >= curpart->life)
            {
				   // particle's time is up
				   // now add it to the free list
				   *part = curpart->next;
				   curpart->next = freeList;
				   freeList = curpart;

				   active--;
			   }
			   else {
               curpart->initTime += delta;
				   curpart->pos += curpart->vel;

				   // run the particle script
				   updateScript->setSelf (curpart, "Particle");
				   updateScript->prepareCall("updateParticle");
				   updateScript->addParam((int)lifetime);
				   updateScript->run (1);

				   part = &curpart->next;
			   }
		   }
	   }

	   if ( delta > emitRate && active < maxActive)
      {
		   for ( int i = 0; i < emitCount; i++ )
         {
			   // fetch a particle from the free list
			   Particle *part = freeList;
			   if (!part)
				   part = new Particle();
			   else
				   freeList = freeList->next;
			   part->next = activeList;
			   activeList = part;

			   // initialize the particle
			   part->pos = getPosition();
			   part->pos.x += rand()%6;
			   part->pos.y += rand()%4;
			   part->vel = Vector (0, -1.0f-rand()%2);
			   part->color = Color(1,1,0);
			   part->initTime = 0;
			   part->life = (rand()%2000) / 1000.0f;
			   part->state = 0;
			   part->size = 20;

			   active++;
		   }
	   }
   }
   else
   {
      setDirty(8);
   }
}

/// \fn ParticleSystem::doDraw ()
/// \brief Draw the particles to the screen buffer via a vertex buffer and
/// vertex shader for rendering speed.
void ParticleSystem::doDraw ()
{
   if ( isReplica() )
   {
      glDisable(GL_DEPTH_TEST);
	   //glDisable(GL_ALPHA_TEST);
	   glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);

      texture->enable ();

	   Uint32 num = 0;
	   Particle* part = activeList;
	   ParticleVertex* verts = (ParticleVertex*)buffer->lock (0);

	   path->enable();
	   while (part)
      {
		   float halfSize = part->size * 0.5f;

		   // create the quad
		   verts[0].pos = part->pos;
		   verts[0].diffuse = part->color;
		   verts[0].tex.set (0,0);
		   verts[0].offset.set (-halfSize,-halfSize);

		   verts[1].pos = part->pos;
		   verts[1].diffuse = part->color;
		   verts[1].tex.set (0,1);
		   verts[1].offset.set (-halfSize,halfSize);

		   verts[2].pos = part->pos;
		   verts[2].diffuse = part->color;
		   verts[2].tex.set (1,1);
		   verts[2].offset.set (halfSize,halfSize);

		   verts[3].pos = part->pos;
		   verts[3].diffuse = part->color;
		   verts[3].tex.set (1,0);
		   verts[3].offset.set (halfSize,-halfSize);

		   verts += 4;

		   // check buffer limit
		   if (++num >= maxBufferSize) {
			   // buffer full -> render contents and continue
			   buffer->unlock ();

			   buffer->enable ();
			   glDrawArrays (GL_QUADS, 0, num*4);
			   buffer->disable ();

			   verts = (ParticleVertex*)buffer->lock (0);
			   num = 0;
		   }

		   part = part->next;
	   }

	   // render any remaining particles
	   buffer->unlock ();
	   if (num != 0) {
		   buffer->enable ();
		   glDrawArrays (GL_QUADS, 0, num*4);
		   buffer->disable ();
	   }
	   path->disable();

	   glDisable(GL_BLEND);
	   glEnable(GL_ALPHA_TEST);

	   texture->disable();
   }
}

void ParticleSystem::pack(BitStream& stream) const
{
   Object::pack(stream);
   stream << true;
}

void ParticleSystem::unpack(BitStream& stream)
{
   Object::unpack(stream);

   bool dirty;
   stream >> dirty;
}
