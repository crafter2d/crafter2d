
#ifndef QUERY_BODY_COMPONENT_MESSAGE_H
#define QUERY_BODY_COMPONENT_MESSAGE_H

#include "core/physics/body.h"
#include "core/core_base.h"

#include "componentmessage.h"

class CORE_API QueryBodyComponentMessage : public ComponentMessage
{
public:
   QueryBodyComponentMessage();
   virtual ~QueryBodyComponentMessage();

   bool hasBody() const;
   Body& getBody();
   void  setBody(Body& body);

private:
   Body* mpBody;
};

#endif // QUERY_BODY_COMPONENT_MESSAGE_H
