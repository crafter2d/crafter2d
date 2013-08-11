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

#include <tinyxml.h>

#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/system/timer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/vertexinputlayout.h"
#include "core/graphics/device.h"
#include "core/graphics/codepath.h"
#include "core/graphics/rendercontext.h"
#include "core/resource/resourcemanager.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "process.h"
#include "nodevisitor.h"

struct ParticleVertex {
	Vector pos;
	Color diffuse;
	Vector tex;
	Vector offset;
};

// - Particle

Particle::Particle():
   pos(),
   vel(),
   color(),
   initTime(0),
   life(0),
   state(0),
   size(0),
   next(NULL)
{
}

// - ParticleSystem

IMPLEMENT_REPLICATABLE(ParticleSystemId, ParticleSystem, Entity)

/// \fn ParticleSystem::ParticleSystem()
/// \brief Initializes member variables.
ParticleSystem::ParticleSystem():
   Entity(),
   position(),
   mEffect(),
	activeList(0),
   freeList(0),
   mGeometryBuffer(NULL),
   mGeometryBufferSize(0),
   updateScript(NULL),
   emitRate(0),
   emitCount(0),
   active(0),
   maxActive(70),
	lastUpdate(0),
   lastInit(0)
{
}

ParticleSystem::~ParticleSystem()
{
   destroy();
}

/*
bool ParticleSystem::load(TiXmlDocument& doc)
{
   Log& log = Log::getInstance();

   // find particle information
   const TiXmlElement* psystem = static_cast<const TiXmlElement*>(doc.FirstChild ("particlesystem"));
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
	const TiXmlElement* pelement = static_cast<const TiXmlElement*>(psystem->FirstChild ("texture"));
	if ( pelement == NULL )
   {
		log.error("ParticleSystem.load: object has no texture.\n");
		return false;
	}

   const TiXmlText* value = static_cast<const TiXmlText*>(pelement->FirstChild());
   texture = ResourceManager::getInstance().getTexture(value->Value());
   if ( texture.isValid() )
      return false;

   // load the update script
   pelement = static_cast<const TiXmlElement*>(psystem->FirstChild("update_script"));
   if ( pelement != NULL )
   {
      const TiXmlText* value = static_cast<const TiXmlText*>(pelement->FirstChild());
      ScriptManager& mgr = getSceneGraph().getProcess().getScriptManager();

	   //updateScript = mgr.createScript();
      //if ( !updateScript->load(value->Value()) )
      //   return false;
   }

   return prepare();
}
*/

/// \fn ParticleSystem::prepare()
/// \brief Creates and initializes the particle system components neccessary for rendering.
bool ParticleSystem::prepare(Graphics::Device& device)
{
   using namespace Graphics;

   // load effect
   VertexInputLayout layout(Graphics::INPUT_XY | Graphics::INPUT_Diffuse | Graphics::INPUT_Tex0 | Graphics::INPUT_Tex1);
   mEffect.load(device, layout, UTEXT("shaders/pointsprite.xml"));

   int usage  = VertexBuffer::eStream | VertexBuffer::eWriteOnly;

	// generate the vertex buffer
   mGeometryBufferSize = 256;
	mGeometryBuffer = device.createVertexBuffer();
	if (!mGeometryBuffer->create(layout, mGeometryBufferSize * 4, usage))
		return false;

	srand(TIMER.getTick() * 1000);
	return true;
}

/// \fn ParticleSystem::destroy()
/// \brief Release all dynamic objects inside this particle system.
void ParticleSystem::destroy()
{
   mEffect.destroy();
   
   if ( mGeometryBuffer != NULL )
   {
      delete mGeometryBuffer;
      mGeometryBuffer = NULL;
   }
   if ( updateScript != 0 )
   {
      delete updateScript;
      updateScript = 0;
   }

   // release the particle lists of this particle system
   while ( activeList != 0 )
   {
      Particle* temp = activeList;
      activeList = activeList->next;
      delete temp;
   }
   while ( freeList != 0 )
   {
      Particle* temp = freeList;
      freeList = freeList->next;
      delete temp;
   }
}

/// \fn ParticleSystem::doUpdate (float delta)
/// \brief Fires the particle update script for every particle. After that the system
/// checks if there are still enough particles alive. If not, new particles are initialized
/// and put in the active list.
void ParticleSystem::doUpdate(float delta)
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
               Variant arg((int)lifetime);
				   updateScript->run(UTEXT("updateParticle"), 1, &arg);

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
			   part->pos = position;
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
void ParticleSystem::doDraw(Graphics::RenderContext& context) const
{
	uint num = 0;
	Particle* part = activeList;
	ParticleVertex* verts = (ParticleVertex*)mGeometryBuffer->lock(context);

   //mEffect.enable(context);

	while (part)
   {
		float halfSize = part->size * 0.5f;

		// create the rectangle
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
      if ( ++num >= mGeometryBufferSize )
      {
			// buffer full -> render contents and continue
			mGeometryBuffer->unlock(context);

			mGeometryBuffer->enable(context);
         context.drawTriangles(0, num * 4);
			mGeometryBuffer->disable(context);

			verts = (ParticleVertex*)mGeometryBuffer->lock (context);
			num = 0;
		}

		part = part->next;
	}

	// render any remaining particles
	mGeometryBuffer->unlock(context);
	if ( num != 0 )
   {
		mGeometryBuffer->enable(context);
		context.drawTriangles(0, num * 4);
		mGeometryBuffer->disable(context);
	}

	//mEffect.disable(context);
}

// - Visitor

void ParticleSystem::accept(NodeVisitor& visitor)
{
   visitor.visitParticleSystem(this);
}

// - Streaming 

void ParticleSystem::doPack(DataStream& stream) const
{
   Entity::doPack(stream);
   stream << true;
}

void ParticleSystem::doUnpack(DataStream& stream)
{
   Entity::doUnpack(stream);

   bool dirty;
   stream >> dirty;
}
