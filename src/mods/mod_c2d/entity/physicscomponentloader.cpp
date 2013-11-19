
#include "physicscomponentloader.h"

#include <tinyxml.h>

#include "core/smartptr/autoptr.h"
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
   AutoPtr<PhysicsComponentDefinitionProto> result = new PhysicsComponentDefinitionProto();

   int isstatic = 0;
   element.QueryIntAttribute("static", &isstatic);
   if ( isstatic > 0 )
      result->mStatic = true;

   int rotate = 1;
   if ( element.QueryIntAttribute("rotate", &rotate) == TIXML_SUCCESS && rotate == 0 )
      result->mFixedRotation = true;

   const TiXmlElement* pshapeelement = dynamic_cast<const TiXmlElement*>(element.FirstChild("shape"));
   if ( pshapeelement != NULL )
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
            
            result->mShape = PhysicsComponentDefinitionProto::eBox;
            result->mInfoA = width / 30;
            result->mInfoB = height / 30;
         }
         else if ( shapetype == UTEXT("circle") )
         {
            float radius = 0.0f;
            pshapeelement->QueryFloatAttribute("radius", &radius);
            
            result->mShape = PhysicsComponentDefinitionProto::eCircle;
            result->mInfoA = radius / 30.0f;
         }
      }
   }

   return result.release();
}
