
#include "entitywriter.h"

#include "core/streams/datastream.h"
#include "core/defines.h"

#include <memory>

#include "proto/entitydefinitionproto.h"
#include "proto/meshcomponentdefinitionproto.h"
#include "proto/physicscomponentdefinitionproto.h"

#include "entityloader.h"
#include "meshcomponentloader.h"
#include "physicscomponentloader.h"

EntityWriter::EntityWriter():
   ContentWriter()
{
}

// - Writing

bool EntityWriter::write(DataStream& stream, const String& filename)
{
   c2d::EntityLoader loader;
   std::unique_ptr<EntityDefinitionProto> definition(loader.load(filename));
   if ( definition )
   {
      definition->write(stream);
      
      return true;
   }

   return false;
}
