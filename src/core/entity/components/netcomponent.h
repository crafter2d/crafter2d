
#ifndef NET_COMPONENT_H
#define NET_COMPONENT_H

#include <cstdint>

#include "component.h"

class DataStream;
class NetStreamComponentMessage;

class CORE_API NetComponent : public Component
{
public:
   NetComponent();

 // overrides
   virtual void registerComponent(Components& components) override;
   virtual void handleMessage(ComponentMessage& message) override;

private:
 // enums
   enum { ePositionDirty = 2, eRotationDirty = 4, eNameDirty = 256 };

 // message handling
   void handleStreamMessage(NetStreamComponentMessage& message);

 // storage
   void writeToStream(DataStream& stream);
   void readFromStream(DataStream& stream);

 // data
   uint32_t mDirtyFlag;
};

#endif // NET_COMPONENT_H
