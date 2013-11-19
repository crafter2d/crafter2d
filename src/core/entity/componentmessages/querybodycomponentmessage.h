
#ifndef QUERY_BODY_COMPONENT_MESSAGE_H
#define QUERY_BODY_COMPONENT_MESSAGE_H

#include "engine/physics/body.h"

#include "componentmessage.h"

class QueryBodyComponentMessage : public ComponentMessage
{
public:
   QueryBodyComponentMessage();

   bool hasBody() const;
   Body& getBody();
   void  setBody(Body& body);

private:
   Body* mpBody;
};

#endif // QUERY_BODY_COMPONENT_MESSAGE_H
