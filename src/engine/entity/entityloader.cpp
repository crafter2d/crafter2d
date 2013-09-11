
#include "entityloader.h"

#include <tinyxml.h>

#include "core/defines.h"

#include "engine/physics/simulator.h"
#include "engine/physics/revolutejointdefinition.h"

#include "entitydefinition.h"
#include "entitylinkdefinition.h"

EntityLoader::EntityLoader():
   mLoaders()
{
}

// - Operations

void EntityLoader::registerLoader(ComponentLoader* ploader)
{
   mLoaders.insert(std::make_pair(ploader->getXmlTag(), ploader));
}

EntityDefinition* EntityLoader::load(const String& filename)
{
   // try to find the object in the file
   TiXmlDocument doc(filename.toUtf8());
   if ( !doc.LoadFile() )
   {
      // throw error;
      return NULL;
   }

	const TiXmlElement* pentity = static_cast<const TiXmlElement*>(doc.FirstChild("entity"));
   if ( pentity == NULL )
   {
      // throw error;
      return NULL;
   }

   return load(*pentity);
}

EntityDefinition* EntityLoader::load(const TiXmlElement& entity)
{
   EntityDefinition* pentitydef = new EntityDefinition();

   // create the actual scripted object
   const std::string* pclasstype = entity.Attribute(std::string("class"));
   pentitydef->setClassName(pclasstype != NULL ? String(*pclasstype) : UTEXT("engine.game.Actor"));

   const TiXmlElement* pelement = NULL;
   for ( const TiXmlElement* pelement = entity.FirstChildElement(); pelement != NULL; pelement = pelement->NextSiblingElement() )
   {
      String name = String::fromUtf8(pelement->Value());

      if ( name == UTEXT("entity") )
      {
         EntityDefinition* pchild = NULL;

         const char* pfilename = pelement->Attribute("file");
         if ( pfilename != NULL )
         {
            String filename = String::fromUtf8(pfilename);
            pchild = load(filename);
         }
         else
         {
            pchild = load(*pelement);
         }

         if ( pchild == NULL )
         {
            // throw and error
         }

         float offsetx, offsety;
         pelement->QueryFloatAttribute("offsetx", &offsetx);
         pelement->QueryFloatAttribute("offsety", &offsety);

         pchild->setOffset(Vector(offsetx, offsety));
         
         pentitydef->addChild(pchild);
      }
      else if ( name == UTEXT("link") )
      {
         loadLink(*pelement, *pentitydef);
      }
      else
      {
         Loaders::iterator it = mLoaders.find(name);
         if ( it != mLoaders.end() )
         {
            ComponentLoader* ploader = it->second;
            ASSERT_PTR(ploader);

            ComponentDefinition* pdefinition = ploader->load(*pelement);
            if ( pdefinition == NULL )
            {
               // throw and error!
            }

            pentitydef->addComponentDefinition(pdefinition);
         }
      }
   }

   return pentitydef;
}

void EntityLoader::loadLink(const TiXmlElement& xmllink, EntityDefinition& definition)
{
   int left, right;
   if ( xmllink.QueryIntAttribute("left", &left) == TIXML_SUCCESS
      && xmllink.QueryIntAttribute("right", &right) == TIXML_SUCCESS )
   {
      JointDefinition* pjoint = NULL;

      const char* ptype = xmllink.Attribute("type");
      if ( ptype != NULL )
      {
         String type = String::fromUtf8(ptype);
         if ( type == UTEXT("revolute" ) )
         {
            float anchorx, anchory;
            xmllink.QueryFloatAttribute("anchorx", &anchorx);
            xmllink.QueryFloatAttribute("anchory", &anchory);

            RevoluteJointDefinition* prevolute = new RevoluteJointDefinition();
            prevolute->anchor.set(anchorx, anchory);

            pjoint = prevolute;
         }
      }

      EntityLinkDefinition* plink = new EntityLinkDefinition(left, right, pjoint);
      definition.addLink(plink);
   }
}
