
#include "physicscomponentloader.h"

#include <tinyxml.h>
#include <memory>

#include "core/string/string.h"
#include "core/defines.h"

#include "proto/physicscomponentdefinitionproto.h"

static const String sXmlTag(UTEXT("body"));

const String& PhysicsComponentLoader::getXmlTag() const
{
   return sXmlTag;
}

ComponentDefinitionProto* PhysicsComponentLoader::load(const TiXmlElement& element)
{
   std::unique_ptr<PhysicsComponentDefinitionProto> result(new PhysicsComponentDefinitionProto());
   BodyDefinition& def = result->mDefinition;
   
   int isstatic = 0;
   element.QueryIntAttribute("static", &isstatic);
   if ( isstatic > 0 )
      def.setStatic(true);

   float mass = 0.0f;
   element.QueryFloatAttribute("mass", &mass);
   def.setMass(mass);

   int rotate = 1;
   if ( element.QueryIntAttribute("rotate", &rotate) == TIXML_SUCCESS && rotate == 0 )
      def.setFixedRotation(true);

   const TiXmlElement* pshapeelement = dynamic_cast<const TiXmlElement*>(element.FirstChild("shape"));
   if ( pshapeelement != nullptr )
   {
      String shapetype = String::fromUtf8(pshapeelement->Attribute("type"));
      if ( !shapetype.isEmpty() )
      {
         if ( shapetype == UTEXT("box") )
         {
            float width;
            float height;

            pshapeelement->QueryFloatAttribute("halfx", &width);
            pshapeelement->QueryFloatAttribute("halfy", &height);
            
            def.setShapeType(BodyDefinition::eBox);
            def.setWidth(width / 30.0f);
            def.setHeight(height / 30.0f);
         }
         else if ( shapetype == UTEXT("circle") )
         {
            float radius = 0.0f;
            pshapeelement->QueryFloatAttribute("radius", &radius);
            
            def.setShapeType(BodyDefinition::eCircle);
            def.setRadius(radius / 30.0f);
         }
      }
   }

   return result.release();
}
