
#include "entityloader.h"

#include <memory>
#include <stdexcept>

#include "core/content/contentmanager.h"
#include "core/physics/revolutejointdefinition.h"
#include "core/defines.h"

#include "proto/childdefinitionproto.h"
#include "proto/entitydefinitionproto.h"
#include "proto/linkdefinitionproto.h"

#include "xml/xmlparser/xmlparser.h"
#include "xml/xmlparser/xmlparts.h"

#include "meshcomponentloader.h"
#include "particlecomponentloader.h"
#include "physicscomponentloader.h"

static String sChild(UTEXT("child"));
static String sEntity(UTEXT("entity"));
static String sLink(UTEXT("link"));

namespace c2d
{
   namespace entity_definitions
   {
      ELEMENT_DEF(child)
         ATTRIBUTE(int, id)
         ATTRIBUTE(std::string, ref)
         ATTRIBUTE(std::string, file)
         ATTRIBUTE(float, offsetx)
         ATTRIBUTE(float, offsety)
      ELEMENT_IMP(child)
         ATTRIBUTE_INIT(id, Attribute::eMandatory, Attribute::eInt, 0);
         ATTRIBUTE_INIT(ref, Attribute::eOptional, Attribute::eString, "");
         ATTRIBUTE_INIT(file, Attribute::eOptional, Attribute::eString, "");
         ATTRIBUTE_INIT(offsetx, Attribute::eMandatory, Attribute::eInt, 0);
         ATTRIBUTE_INIT(offsety, Attribute::eMandatory, Attribute::eFloat, 0.0f);
      ELEMENT_END

      ELEMENT_DEF(link)
         ATTRIBUTE(int, left)
         ATTRIBUTE(int, right)
         ATTRIBUTE(float, anchorx)
         ATTRIBUTE(float, anchory)
         ATTRIBUTE(std::string, type)
      ELEMENT_IMP(link)
         ATTRIBUTE_INIT(left, Attribute::eMandatory, Attribute::eInt, 0)
         ATTRIBUTE_INIT(right, Attribute::eMandatory, Attribute::eInt, 0)
         ATTRIBUTE_INIT(anchorx, Attribute::eMandatory, Attribute::eFloat, 0.0f)
         ATTRIBUTE_INIT(anchory, Attribute::eMandatory, Attribute::eFloat, 0.0f)
         ATTRIBUTE_INIT(type, Attribute::eMandatory, Attribute::eString, "")
      ELEMENT_END

      ELEMENT_DEF(entity)

         ATTRIBUTE(std::string, name)
         ATTRIBUTE(std::string, klass)

         CHILD(entity)
         CHILD(sprite)
         CHILD(child)
         CHILD(link)
         CHILD(body)
         CHILD(particlesystem)

      ELEMENT_IMP(entity)

         ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
         ATTRIBUTE_INIT_NAMED(klass, class, Attribute::eOptional, Attribute::eString, "engine.game.Entity")

         CHILD_INIT(entity, Child::eZeroOrAny)
         CHILD_INIT(sprite, Child::eZeroOrAny)
         CHILD_INIT(child, Child::eZeroOrAny)
         CHILD_INIT(link, Child::eZeroOrAny)
         CHILD_INIT(body, Child::eZeroOrAny)
         CHILD_INIT(particlesystem, Child::eZeroOrAny)

      ELEMENT_END
   }

   EntityLoader::EntityLoader()
   {
   }

   // - Loading

   EntityDefinitionProto* EntityLoader::load(const String& filename)
   {
      XmlParser parser;
      entity_definitions::entity ent;

      if ( parser.parse(ent, filename) )
      {
         return createDefinition(ent);
      }

      throw std::runtime_error("parsing failed");
   }

   EntityDefinitionProto* EntityLoader::createDefinition(const entity_definitions::entity& ent)
   {
      auto pentitydef = std::make_unique<EntityDefinitionProto>();
      pentitydef->mName = ent.name;
      pentitydef->mClassName = ent.klass;

      MeshComponentLoader::load(*pentitydef, ent.sprites);
      PhysicsComponentLoader::load(*pentitydef, ent.bodys);
      ParticleComponentLoader::load(*pentitydef, ent.particlesystems);

      loadChildDefinitions(*pentitydef, ent.childs);
      loadLinkDefinitions(*pentitydef, ent.links);
      loadEntityDefinitions(*pentitydef, ent.entitys);

      return pentitydef.release();
   }

   void EntityLoader::loadEntityDefinitions(EntityDefinitionProto& entity, const std::vector<entity_definitions::entity>& entities)
   {
      for ( auto& ent : entities )
      {
         auto pchildentity = createDefinition(ent);
         entity.mEntities.push_back(pchildentity);
      }
   }

   void EntityLoader::loadChildDefinitions(EntityDefinitionProto& entity, const std::vector<entity_definitions::child>& children)
   {
      for ( auto& child : children )
      {
         auto pchild = std::make_unique<ChildDefinitionProto>();
         pchild->mID = child.id;

         if ( !child.ref.empty() )
         {
            pchild->mRefType = ChildDefinitionProto::eLocalReference;
            pchild->mRef = child.ref;
         }
         else if ( !child.file.empty() )
         {
            pchild->mRefType = ChildDefinitionProto::eFileReference;
            pchild->mRef = child.file;
         }
         else
         {
            throw std::runtime_error("Should either have a file or local reference!");
         }

         pchild->mOffset = Vector(child.offsetx, child.offsety);

         entity.mChildren.push_back(pchild.release());
      }
   }

   void EntityLoader::loadLinkDefinitions(EntityDefinitionProto& entity, const std::vector<entity_definitions::link>& links)
   {
      for ( auto& link : links )
      {
         auto presult = std::make_unique<LinkDefinitionProto>();
         presult->mLeft = link.left;
         presult->mRight = link.right;

         JointDefinition* pjoint = nullptr;
         if ( link.type == "revolute" )
         {
            RevoluteJointDefinition* prevolute = new RevoluteJointDefinition();
            prevolute->anchor.set(link.anchorx, link.anchory);

            pjoint = prevolute;
         }

         presult->mpJointDef = pjoint;
   
         entity.mLinks.push_back(presult.release());
      }
   }
}
