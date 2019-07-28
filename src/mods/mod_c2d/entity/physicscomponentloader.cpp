
#include "physicscomponentloader.h"

#include "core/string/string.h"
#include "core/defines.h"

#include "proto/entitydefinitionproto.h"
#include "proto/physicscomponentdefinitionproto.h"

namespace c2d
{

   // - static 

   void PhysicsComponentLoader::load(EntityDefinitionProto& entity, const std::vector<entity_definitions::body>& bodies)
   {
      PhysicsComponentLoader loader;
      for ( auto& body : bodies )
      {
         auto pcomponent = loader.load(body);
         entity.mComponents.push_back(pcomponent);
      }
   }

   ComponentDefinitionProto* PhysicsComponentLoader::load(const entity_definitions::body& element)
   {
      auto result = std::make_unique<PhysicsComponentDefinitionProto>();
      BodyDefinition& def = result->mDefinition;

      def.setStatic(element.statik);
      def.setFixedRotation(!element.rotate);
      def.setMass(element.mass);

      ASSERT(element.shapes.size() == 1);
      auto& shape = element.shapes[0];

      if ( shape.type == "box" )
      {
         def.setShapeType(BodyDefinition::eBox);
         def.setWidth(shape.halfx / 30.0f);
         def.setHeight(shape.halfy / 30.0f);
      }
      else if ( shape.type == "circle" )
      {
         def.setShapeType(BodyDefinition::eCircle);
         def.setRadius(shape.radius / 30.0f);
      }
      else
      {
         // invalid type
      }

      return result.release();
   }

}
