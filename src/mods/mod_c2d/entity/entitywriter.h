
#ifndef ENTITY_COMPILER_H
#define ENTITY_COMPILER_H

#include "core/content/contentwriter.h"

class ChildDefinitionProto;
class EntityDefinitionProto;
class LinkDefinitionProto;

class EntityWriter : public IContentWriter
{
public:
   EntityWriter();

 // writing
   virtual bool write(DataStream& stream, const String& filename) override;
};

#endif // ENTITY_COMPILER_H
