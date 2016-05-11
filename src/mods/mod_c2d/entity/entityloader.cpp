
#include "entityloader.h"

#include <tinyxml.h>
#include <memory>
#include <stdexcept>

#include "core/content/contentmanager.h"
#include "core/physics/revolutejointdefinition.h"
#include "core/defines.h"

#include "proto/childdefinitionproto.h"
#include "proto/entitydefinitionproto.h"
#include "proto/linkdefinitionproto.h"

#include "meshcomponentloader.h"
#include "particlecomponentloader.h"
#include "physicscomponentloader.h"

static String sChild(UTEXT("child"));
static String sEntity(UTEXT("entity"));
static String sLink(UTEXT("link"));

EntityLoader::EntityLoader():
   mLoaders()
{
  registerLoader(new MeshComponentLoader());
  registerLoader(new ParticleComponentLoader());
  registerLoader(new PhysicsComponentLoader());
}

// - Operations

void EntityLoader::registerLoader(ComponentLoader* ploader)
{
   mLoaders.insert(std::make_pair(ploader->getXmlTag(), ploader));
}

// - Loading

EntityDefinitionProto* EntityLoader::load(const String& filename)
{
   std::string name = filename.toUtf8();
   TiXmlDocument doc(name.c_str());
   if ( !doc.LoadFile() )
   {
      return NULL;
   }

   TiXmlElement* pbase = doc.FirstChildElement("entity");
   if ( pbase == NULL )
   {
      return NULL;
   }
   
   return loadDefinition(*pbase);
}

EntityDefinitionProto* EntityLoader::loadDefinition(const TiXmlElement& entity)
{
   EntityDefinitionProto* pentitydef = new EntityDefinitionProto();

   const char* pname = entity.Attribute("name");
   pentitydef->mName = String::fromUtf8(pname);

   const char* pclasstype = entity.Attribute("class");
   pentitydef->mClassName = (pclasstype != NULL ? String::fromUtf8(pclasstype) : UTEXT("engine.game.Entity"));

   for ( const TiXmlElement* pelement = entity.FirstChildElement(); pelement != NULL; pelement = pelement->NextSiblingElement() )
   {
      String name = String::fromUtf8(pelement->Value());

      if ( name == sChild )
      {
         loadChildDefinition(*pentitydef, *pelement);
      }
      else if ( name == sLink )
      {
         loadLinkDefinition(*pentitydef, *pelement);
      }
      else if ( name == sEntity )
      {
         EntityDefinitionProto* pentity = loadDefinition(*pelement);
         pentitydef->mEntities.push_back(pentity);
      }
      else
      {
         Loaders::iterator it = mLoaders.find(name);
         if ( it != mLoaders.end() )
         {
            ComponentLoader* ploader = it->second;
            ASSERT_PTR(ploader);

            ComponentDefinitionProto* pdefinition = ploader->load(*pelement);
            if ( pdefinition == NULL )
            {
               // throw and error!
            }

            pentitydef->mComponents.push_back(pdefinition);
         }
      }
   }

   return pentitydef;
}

void EntityLoader::loadChildDefinition(EntityDefinitionProto& entity, const TiXmlElement& xmlchild)
{
   std::unique_ptr<ChildDefinitionProto> pchild(new ChildDefinitionProto());

   const char* pid = xmlchild.Attribute("id");
   pchild->mID = String::fromUtf8(pid);

   const char* pref = xmlchild.Attribute("ref");
   if ( pref != NULL )
   {
      pchild->mRefType = ChildDefinitionProto::eLocalReference;
      pchild->mRef = String::fromUtf8(pref);
   }
   else
   {
      const char* pfile = xmlchild.Attribute("file");
      if ( pfile == NULL )
      {
         throw std::runtime_error("Should have a file reference!");
      }

      pchild->mRefType = ChildDefinitionProto::eFileReference;
      pchild->mRef = String::fromUtf8(pfile);
   }

   float offsetx, offsety;
   xmlchild.QueryFloatAttribute("offsetx", &offsetx);
   xmlchild.QueryFloatAttribute("offsety", &offsety);
   pchild->mOffset = Vector(offsetx, offsety);

   entity.mChildren.push_back(pchild.release());
}

void EntityLoader::loadLinkDefinition(EntityDefinitionProto& entity, const TiXmlElement& xmllink)
{
   LinkDefinitionProto* presult = NULL;
   int left, right;

   if ( xmllink.QueryIntAttribute("left", &left) == TIXML_SUCCESS
     && xmllink.QueryIntAttribute("right", &right) == TIXML_SUCCESS )
   {
      const char* ptype = xmllink.Attribute("type");
      if ( ptype == NULL )
      {
         // throw error!!
      }

      String type = String::fromUtf8(ptype);
      
      JointDefinition* pjoint = NULL;
      if ( type == UTEXT("revolute" ) )
      {
         float anchorx, anchory;
         xmllink.QueryFloatAttribute("anchorx", &anchorx);
         xmllink.QueryFloatAttribute("anchory", &anchory);

         RevoluteJointDefinition* prevolute = new RevoluteJointDefinition();
         prevolute->anchor.set(anchorx, anchory);

         pjoint = prevolute;
      }

      presult = new LinkDefinitionProto();
      presult->mLeft = left;
      presult->mRight = right;
      presult->mpJointDef = pjoint;
   }

   entity.mLinks.push_back(presult);
}
