
#include "meshcomponentloader.h"

#include "core/string/string.h"

#include "meshcomponentdefinition.h"

static String sId(UTEXT("mesh"));

MeshComponentLoader::MeshComponentLoader():
   ComponentLoader()
{
}

// - Overrides

const String& MeshComponentLoader::getId() const
{
   return sId;
}

ComponentDefinition* MeshComponentLoader::load(const TiXmlElement& element)
{
   return new MeshComponentDefinition();
}
