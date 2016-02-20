
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

   const TiXmlElement* pXmlAnimation = element.FirstChildElement("animations");
   if ( pXmlAnimation != NULL )
   {
      // query the animation speed (in mm)
      if ( pXmlAnimation->QueryFloatAttribute("speed", &pdefinition->mAnimationSpeed) != TIXML_SUCCESS )
		   pdefinition->mAnimationSpeed = 100;
      pdefinition->mAnimationSpeed /= 1000.0f;

      for ( auto panim = pXmlAnimation->FirstChildElement("anim"); panim != NULL; panim = panim->NextSiblingElement("anim") )
      {
         MeshComponentDefinitionProto::Animation animation;

         std::string name;
         panim->QueryStringAttribute("name", &name);
         animation.name.setToUtf8(name);

         for ( auto panimtile = panim->FirstChildElement("tile"); panimtile != nullptr; panimtile = panimtile->NextSiblingElement("tile") )
         {
            std::string name;            
            if ( panimtile->QueryStringAttribute("name", &name) == TIXML_SUCCESS )
            {
               animation.frames.push_back(String(name));
            }
         }

         pdefinition->mAnimations.push_back(std::move(animation));
      }
   }

   return pdefinition.release();
}
