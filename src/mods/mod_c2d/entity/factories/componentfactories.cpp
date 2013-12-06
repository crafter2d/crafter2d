
#include "componentfactories.h"

#include <cstdint>

#include "../proto/componentdefinitionproto.h"

#include "componentfactory.h"

// - Instantiation

Component* ComponentFactories::instantiate(ComponentDefinitionProto& definition)
{
   for ( uint32_t index = 0; index < size(); ++index )
   {
      ComponentFactory* pfactory = operator[](index);
      if ( pfactory->getComponentType() == definition.getType() )
      {
         return pfactory->instantiate(definition);
      }
   }

   return NULL;
}