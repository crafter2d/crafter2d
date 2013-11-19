
#include "entitywriter.h"

#include <tinyxml.h>

#include "core/entity/entitydefinition.h"
#include "core/streams/datastream.h"
#include "core/defines.h"

#include "proto/entitydefinitionproto.h"
#include "proto/meshcomponentdefinitionproto.h"
#include "proto/physicscomponentdefinitionproto.h"

#include "entityloader.h"
#include "meshcomponentloader.h"
#include "physicscomponentloader.h"

EntityWriter::EntityWriter():
   IContentWriter()
{
}

// - Writing

bool EntityWriter::write(DataStream& stream, const String& filename)
{
   EntityLoader loader;
   EntityDefinitionProto* pdefinition = loader.load(filename);
   if ( pdefinition != NULL )
   {
      pdefinition->write(stream);
   }

   return true;
}
