
#include "meshcomponentdefinition.h"

#include "core/defines.h"

#include "engine/animator.h"

#include <tinyxml.h>

void MeshComponentDefinition::load(const TiXmlElement& element)
{
   if ( element.QueryIntAttribute ("width", &mWidth) != TIXML_SUCCESS ||
		  element.QueryIntAttribute ("height", &mHeight) != TIXML_SUCCESS )
   {
      // throw error
   }

   const TiXmlElement* ptextureelement = element.FirstChildElement("texture");
   if ( ptextureelement != NULL )
   {
      const TiXmlText* pvalue = dynamic_cast<const TiXmlText*>(ptextureelement->FirstChild());
      if ( pvalue == NULL )
      {
         // throw error;
      }

      mTexture = pvalue->ValueStr();
   }

   const TiXmlElement* pXmlAnimation = element.FirstChildElement("animations");
   if ( pXmlAnimation != NULL )
   {
      mpAnimator = Animator::construct(*pXmlAnimation);
      if ( mpAnimator == NULL )
      {
         // throw error;
      }
   }
}

Component* MeshComponentDefinition::instantiate() const
{
   return NULL;
}
