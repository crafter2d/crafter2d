
#include "physicscomponentloader.h"

#include <tinyxml.h>

#include "core/smartptr/autoptr.h"
#include "core/string/string.h"
#include "core/defines.h"

#include "physicscomponentdefinition.h"

static const String sXmlTag(UTEXT("body"));

const String& PhysicsComponentLoader::getXmlTag() const
{
   return sXmlTag;
}

ComponentDefinition* PhysicsComponentLoader::load(const TiXmlElement& element)
{
   AutoPtr<PhysicsComponentDefinition> result = new PhysicsComponentDefinition();

   BodyDefinition& bodydef = result->getBodyDefinition();

   int isstatic = 0;
   element.QueryIntAttribute("static", &isstatic);
   if ( isstatic > 0 )
      bodydef.setStatic(true);

   int rotate = 1;
   if ( element.QueryIntAttribute("rotate", &rotate) == TIXML_SUCCESS && rotate == 0 )
      bodydef.setFixedRotation(true);

   const TiXmlElement* pshapeelement = dynamic_cast<const TiXmlElement*>(element.FirstChild("shape"));
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

            bodydef.setShapeType(BodyDefinition::eBox);
            bodydef.setWidth(width);
            bodydef.setHeight(height);
         }
         else if ( pshapetype->compare("circle") == 0 )
         {
            float radius = 0.0f;
            pshapeelement->QueryFloatAttribute("radius", &radius);
            
            bodydef.setShapeType(BodyDefinition::eCircle);
            bodydef.setRadius(radius / 30.0f);
         }
      }
   }

   return result.release();
}
