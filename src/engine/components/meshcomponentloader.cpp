
#include "meshcomponentloader.h"

#include <tinyxml.h>

#include "core/string/string.h"
#include "core/smartptr/autoptr.h"
#include "core/graphics/animator.h"

#include "meshcomponentdefinition.h"

static String sId(UTEXT("sprite"));

using namespace Graphics;

MeshComponentLoader::MeshComponentLoader(Graphics::Device& device):
   ComponentLoader(),
   mDevice(device)
{
}

// - Overrides

const String& MeshComponentLoader::getXmlTag() const
{
   return sId;
}

ComponentDefinition* MeshComponentLoader::load(const TiXmlElement& element)
{
   AutoPtr<MeshComponentDefinition> pdefinition = new MeshComponentDefinition();
   SpriteDefinition& spritedef = pdefinition->getSpriteDefinition();

   int width, height;

   if ( element.QueryIntAttribute ("width", &width) != TIXML_SUCCESS ||
		  element.QueryIntAttribute ("height", &height) != TIXML_SUCCESS )
   {
      // throw error
   }

   spritedef.setSize(Size(width, height));

   const TiXmlElement* ptextureelement = element.FirstChildElement("texture");
   if ( ptextureelement != NULL )
   {
      const TiXmlText* pvalue = dynamic_cast<const TiXmlText*>(ptextureelement->FirstChild());
      if ( pvalue == NULL )
      {
         // throw error;
      }

      String texturefile = String(pvalue->ValueStr());
      TexturePtr texture = ResourceManager::getInstance().getTexture(mDevice, texturefile);
      if ( !texture.isValid() )
      {
         // throw error
      }

      spritedef.setTexture(texture);
   }

   const TiXmlElement* pXmlAnimation = element.FirstChildElement("animations");
   if ( pXmlAnimation != NULL )
   {
      Animator* panimator = Animator::construct(*pXmlAnimation);
      if ( panimator == NULL )
      {
         // throw error;
      }

      panimator->initialize(spritedef.getTexture(), spritedef.getSize());

      spritedef.setSpriteAnimator(panimator);
   }

   return pdefinition.release();
}
