
#include "meshcomponentloader.h"

#include <memory>
#include <tinyxml.h>

#include "core/string/string.h"
#include "core/smartptr/autoptr.h"
#include "core/graphics/animator.h"

#include "proto/meshcomponentdefinitionproto.h"

static String sId(UTEXT("sprite"));

using namespace Graphics;

MeshComponentLoader::MeshComponentLoader():
   ComponentLoader()
{
}

// - Overrides

const String& MeshComponentLoader::getXmlTag() const
{
   return sId;
}

ComponentDefinitionProto* MeshComponentLoader::load(const TiXmlElement& element)
{
   std::unique_ptr<MeshComponentDefinitionProto> pdefinition(new MeshComponentDefinitionProto());

   int width, height;
   if ( element.QueryIntAttribute ("width", &width) != TIXML_SUCCESS ||
		  element.QueryIntAttribute ("height", &height) != TIXML_SUCCESS )
   {
      // throw error
   }

   pdefinition->mWidth = width;
   pdefinition->mHeight = height;

   const TiXmlElement* ptextureelement = element.FirstChildElement("texture");
   if ( ptextureelement != NULL )
   {
      const TiXmlText* pvalue = dynamic_cast<const TiXmlText*>(ptextureelement->FirstChild());
      if ( pvalue == NULL )
      {
         // throw error;
      }

      pdefinition->mTexture.setToUtf8(pvalue->Value());
   }

   const TiXmlElement* pXmlAnimation = element.FirstChildElement("animations");
   if ( pXmlAnimation != NULL )
   {
      // query the animation speed (in mm)
      if ( pXmlAnimation->QueryFloatAttribute("speed", &pdefinition->mAnimationSpeed) != TIXML_SUCCESS )
		   pdefinition->mAnimationSpeed = 100;
      pdefinition->mAnimationSpeed /= 1000.0f;

      const TiXmlElement* panim = NULL;
      for ( panim = pXmlAnimation->FirstChildElement("anim"); panim != NULL; panim = panim->NextSiblingElement("anim") )
      {
         int length;
         if ( panim->QueryIntAttribute("length", &length) == TIXML_SUCCESS )
         {
            pdefinition->mAnimations.push_back(length);
         }
      }
   }

   return pdefinition.release();
}
