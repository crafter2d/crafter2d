
#ifndef ENTITY_READER_H
#define ENTITY_READER_H

#include "core/content/contentreader.h"

class ComponentFactories;

class EntityReader : public ContentReader
{
protected:
 // overrides
   virtual IContent* read(DataStream& stream) override;

private:
 // helpers
   void setupFactories(ComponentFactories& factories);
};

#endif // ENTITY_READER_H
