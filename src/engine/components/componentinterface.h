
#ifndef ENTITY_COMPONENT_INTERFACE_H
#define ENTITY_COMPONENT_INTERFACE_H

namespace ComponentInterface
{
   enum ComponentType
   {
      eMeshComponent,
      ePhysisComponent
   };

   enum ComponentMessageType
   {
	   ePositionChangedMsg,
	   eNUM_MESSAGE_TYPES
   };
};

#endif // ENTITY_COMPONENT_INTERFACE_H
