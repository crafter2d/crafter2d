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
#include "objectloader.h"

#include <tinyxml.h>

#include "core/log/log.h"
#include "core/smartptr/autoptr.h"

#include "engine/physics/body.h"
#include "engine/physics/simulator.h"
#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"
#include "engine/resource/resourcemanager.h"
#include "engine/animator.h"
#include "engine/actor.h"

#include "engine/process.h"
#include "engine/script/scriptmanager.h"

#include "invalidcontentexception.h"

ActorLoader::ActorLoader(Process& process):
   ContentLoader(process)
{
}

// - Operations

Actor* ActorLoader::load(const std::string& filename)
{
   Log& log = Log::getInstance();

	// try to find the object in the file
   TiXmlDocument doc(filename);
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

   // check if we need to create the object
   AutoPtr<Actor> actor = new Actor();

   // find the name and dimensions of the object
   actor->setName(pobject->Attribute("name"));
   actor->setFilename(filename);

   int width, height;
	if ( pobject->QueryIntAttribute ("width", &width) != TIXML_SUCCESS ||
		   pobject->QueryIntAttribute ("height", &height) != TIXML_SUCCESS )
   {
      throw new InvalidContentException("ActorLoader: actor requires a valid size.");
	}

   // see whether or not the object is static
   int temp = 0;
   bool staticobj = false;
   if ( pobject->QueryIntAttribute("static", &temp) == TIXML_SUCCESS )
      staticobj = (temp == 1);

   actor->setSize(width, height);
   actor->setStatic(staticobj);

	// load texture data
	const TiXmlElement* ptex = static_cast<const TiXmlElement*>(pobject->FirstChild ("texture"));
	if ( ptex != NULL )
   {
		TiXmlText* pvalue = (TiXmlText*)ptex->FirstChild();
      TexturePtr texture = ResourceManager::getInstance().getTexture(pvalue->ValueStr());
      if ( !texture.isValid() )
      {
         throw new InvalidContentException("ActorLoader: can not load resource " + pvalue->ValueStr());
      }
      else
      {
         actor->setTexture(texture);
      }
	}
   else
   {
      throw new InvalidContentException("ActorLoader: actor requires a texture (missing in " + actor->getName() + ")");
	}

   // load animation stuff
   Animator* panimator = Animator::construct(*pobject, *actor);
   actor->setAnimator(panimator);

   // load simulation info
   if ( Body::hasInfo(*pobject) )
   {
      Body& body = getSimulator().createBody(*actor);
      body.load(*pobject);

      actor->setBody(body);
   }

   // create the actual scripted object
   const std::string* pclasstype = pobject->Attribute(std::string("type"));
   actor->setType(pclasstype != NULL ? *pclasstype : "engine.game.Actor");

   AutoPtr<Script> script = getProcess().getScriptManager().loadNative(actor->getType(), actor.getPointer(), false);
   if ( !script.hasPointer() )
   {
      throw new InvalidContentException("Type " + *pclasstype + " is not a valid class name.");
   }
   script->run("onCreated");
   
   return actor.release();
}
