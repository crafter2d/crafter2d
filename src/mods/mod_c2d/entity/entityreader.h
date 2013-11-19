
#ifndef ENTITY_READER_H
#define ENTITY_READER_H

#include "core/content/contentreader.h"

class ComponentFactories;

class EntityReader : public IContentReader
{
protected:
 // overrides
   virtual IContent* read(ContentManager& manager, DataStream& stream) override;

private:
 // helpers
   void setupFactories(ComponentFactories& factories);
};

#endif // ENTITY_READER_H
