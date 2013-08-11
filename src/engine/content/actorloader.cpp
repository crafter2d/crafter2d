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

#include "engine/physics/bodydefinition.h"
#include "engine/physics/simulator.h"
#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"
#include "engine/sprites/sprite.h"
#include "engine/animator.h"
#include "engine/entity.h"
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

Entity* ActorLoader::load(const String& filename, int flags)
{
   Log& log = Log::getInstance();

	// try to find the object in the file
   TiXmlDocument doc(filename.toUtf8());
   if ( !doc.LoadFile() )
   {
      return NULL;
   }

	const TiXmlElement* pobject = static_cast<const TiXmlElement*>(doc.FirstChild("entity"));
	if ( pobject == NULL )
   {
      log.error("Object.load: Invalid XML file format, object expected.\n");
		return NULL;
	}

   AutoPtr<Entity> actor = new Entity();
   actor->setName(String::fromUtf8(pobject->Attribute("name")));
   actor->setFilename(filename);

   // create the actual scripted object
   const std::string* pclasstype = pobject->Attribute(std::string("type"));
   actor->setClassName(pclasstype != NULL ? String(*pclasstype) : UTEXT("engine.game.Actor"));

   if ( IS_SET(flags, ContentLoader::eLoadPhysics) )
   {
      const TiXmlElement* pphysicsxml = pobject->FirstChildElement("body");
      if ( pphysicsxml != NULL )
      {
         loadPhysics(*pphysicsxml, *actor);
      }
   }

   if ( IS_SET(flags, ContentLoader::eLoadGraphics) )
   {
      const TiXmlElement* pspritexml = pobject->FirstChildElement("sprite");
      if ( pspritexml != NULL )
      {
         loadMesh(*pspritexml, *actor);
      }
      else
      {
         Log::getInstance().error("Could not load the mesh for object..");
      }
   }
   
   return actor.release();
}

void ActorLoader::loadPhysics(const TiXmlElement& physicsxml, Entity& actor)
{
   AutoPtr<BodyDefinition> definition = new BodyDefinition();

   int isstatic = 0;
   physicsxml.QueryIntAttribute("static", &isstatic);
   if ( isstatic > 0 )
      definition->setStatic(true);

   int rotate = 1;
   if ( physicsxml.QueryIntAttribute("rotate", &rotate) == TIXML_SUCCESS && rotate == 0 )
      definition->setFixedRotation(true);

   const TiXmlElement* pshapeelement = dynamic_cast<const TiXmlElement*>(physicsxml.FirstChild("shape"));
   if ( pshapeelement != NULL )
   {
      const std::string* pshapetype = pshapeelement->Attribute(std::string("type"));
      if ( pshapetype != NULL )
      {
         if ( pshapetype->compare("box") == 0 )
         {
            float width;
            float height;

            pshapeelement->QueryFloatAttribute("halfx", &width);
            pshapeelement->QueryFloatAttribute("halfy", &height);

            width /= 30.0f;
            height /= 30.0f;

            definition->setShapeType(BodyDefinition::eBox);
            definition->setWidth(width);
            definition->setHeight(height);
         }
         else if ( pshapetype->compare("circle") == 0 )
         {
            float radius = 0.0f;
            pshapeelement->QueryFloatAttribute("radius", &radius);
            
            definition->setShapeType(BodyDefinition::eCircle);
            definition->setRadius(radius / 30.0f);
         }
      }
   }
}

void ActorLoader::loadMesh(const TiXmlElement& mesh, Entity& actor)
{
   int width, height;
	if ( mesh.QueryIntAttribute ("width", &width) != TIXML_SUCCESS ||
		  mesh.QueryIntAttribute ("height", &height) != TIXML_SUCCESS )
   {
      throw new InvalidContentException(UTEXT("ActorLoader: actor requires a valid size."));
	}

   AutoPtr<Sprite> sprite = new Sprite(); 

   AutoPtr<MeshComponent> meshcomp = new MeshComponent();
   sprite->setSize(Size(width, height));

   // load texture
   const TiXmlElement* ptextureelement = mesh.FirstChildElement("texture");
   if ( ptextureelement != NULL )
   {
      const TiXmlText* pvalue = dynamic_cast<const TiXmlText*>(ptextureelement->FirstChild());
      ASSERT_PTR(pvalue);
      String texture(pvalue->ValueStr());
      /*
      TexturePtr ptr = ResourceManager::getInstance().getTexture(device, texture);
      if ( !ptr.isValid() )
      {
         throw new InvalidContentException(UTEXT("ActorLoader: can not load texture ") + texture);
      }
      
      sprite->setTexture(ptr);
      */
   }
   else
   {
      throw new InvalidContentException(UTEXT("ActorLoader: actor requires a texture (missing in ") + actor.getName() + ')');
   }

   // load animation stuff
   const TiXmlElement* pXmlAnimation = mesh.FirstChildElement("animations");
   if ( pXmlAnimation != NULL )
   {
      //mpAnimator = Animator::construct(*pXmlAnimation);
   }
}
