
#include "entityloader.h"

#include <tinyxml.h>

#include "core/defines.h"

#include "entitydefinition.h"

EntityLoader::EntityLoader():
   mLoaders()
{
}

// - Operations

void EntityLoader::registerLoader(ComponentLoader* ploader)
{
   mLoaders.insert(std::make_pair(ploader->getId(), ploader));
}

void EntityLoader::load(const String& filename)
{
   // try to find the object in the file
   TiXmlDocument doc(filename.toUtf8());
   if ( !doc.LoadFile() )
   {
      // throw error;
   }

	const TiXmlElement* pentity = static_cast<const TiXmlElement*>(doc.FirstChild("entity"));
   if ( pentity == NULL )
   {
      // throw error;
   }

   EntityDefinition* pentitydef = new EntityDefinition();

   const TiXmlNode* pcomponent = NULL;
   while( pcomponent = pentity->IterateChildren(pcomponent) )
   {
      const TiXmlElement* pcomponentelement = pcomponent->ToElement();
      if ( pcomponentelement == NULL )
      {
         break;
      }

      String componentname = String::fromUtf8(pcomponentelement->Value());
      Loaders::iterator it = mLoaders.find(componentname);
      if ( it != mLoaders.end() )
      {
         ComponentLoader* ploader = it->second;
         ASSERT_PTR(ploader);

         ComponentDefinition* pdefinition = ploader->load(*pcomponentelement);
         ASSERT_PTR(pdefinition); // otherwise an exception should have been thrown

         pentitydef->addComponentDefinition(pdefinition);
      }
   }
}
