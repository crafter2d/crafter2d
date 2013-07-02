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
#include "actorloader.h"

#include <tinyxml.h>

#include "core/log/log.h"
#include "core/resource/resourcemanager.h"
#include "core/smartptr/autoptr.h"

#include "engine/physics/body.h"
#include "engine/physics/simulator.h"
#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"
#include "engine/animator.h"
#include "engine/actor.h"
#include "engine/components/physicscomponent.h"
#include "engine/components/meshcomponent.h"
#include "engine/process.h"
#include "engine/script/scriptmanager.h"

#include "invalidcontentexception.h"

ActorLoader::ActorLoader(Process& process):
   ContentLoader(process)
{
}

// - Operations

Actor* ActorLoader::load(const String& filename, int flags)
{
   Log& log = Log::getInstance();

	// try to find the object in the file
   TiXmlDocument doc(filename.toUtf8());
   if ( !doc.LoadFile() )
   {
      return NULL;
   }

	const TiXmlElement* pobject = static_cast<const TiXmlElement*>(doc.FirstChild ("object"));
	if ( pobject == NULL )
   {
      log.error("Object.load: Invalid XML file format, object expected.\n");
		return NULL;
	}

   AutoPtr<Actor> actor = new Actor();
   actor->setName(String::fromUtf8(pobject->Attribute("name")));
   actor->setFilename(filename);

   // create the actual scripted object
   const std::string* pclasstype = pobject->Attribute(std::string("type"));
   actor->setClassName(pclasstype != NULL ? String(*pclasstype) : UTEXT("engine.game.Actor"));

   if ( IS_SET(flags, ContentLoader::eLoadPhysics) )
   {
      loadPhysics(*pobject, *actor);
   }

   if ( IS_SET(flags, ContentLoader::eLoadGraphics) )
   {
      const TiXmlElement* pmesh = pobject->FirstChildElement("mesh");
      if ( pmesh != NULL )
      {
         loadMesh(*pmesh, *actor);
      }
      else
      {
         Log::getInstance().error("Could not load the mesh for object..");
      }
   }
   
   return actor.release();
}

void ActorLoader::loadPhysics(const TiXmlElement& object, Actor& actor)
{
   if ( Body::hasInfo(object) )
   {
      Body& body = getSimulator().createBody(actor);
      body.load(object);

      PhysicsComponent* ppc = new PhysicsComponent();
      ppc->setBody(body);

      actor.addComponent(ppc);
   }
}

void ActorLoader::loadMesh(const TiXmlElement& mesh, Actor& actor)
{
   int width, height;
	if ( mesh.QueryIntAttribute ("width", &width) != TIXML_SUCCESS ||
		  mesh.QueryIntAttribute ("height", &height) != TIXML_SUCCESS )
   {
      throw new InvalidContentException(UTEXT("ActorLoader: actor requires a valid size."));
	}

   AutoPtr<MeshComponent> meshcomp = new MeshComponent();
   meshcomp->setSize(Size(width, height));

   // load texture data
   const TiXmlElement* peffect = mesh.FirstChildElement("effect");
	if ( peffect != NULL )
   {
      const TiXmlText* pvalue = dynamic_cast<const TiXmlText*>(peffect->FirstChild());
      const std::string& name = pvalue->ValueStr();
      String effectName(name.c_str());
      meshcomp->setEffectName(effectName);
	}
   else
   {
      throw new InvalidContentException(UTEXT("ActorLoader: actor requires a texture (missing in ") + actor.getName() + ')');
	}

   // load animation stuff
   const TiXmlElement* pXmlAnimation = mesh.FirstChildElement("animations");
   if ( pXmlAnimation != NULL )
   {
      Animator* panimator = Animator::construct(*pXmlAnimation);
      meshcomp->setAnimator(panimator);
   }

   actor.addComponent(meshcomp.release());
}
