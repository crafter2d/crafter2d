
#include "field.h"

#include "core/defines.h"

#include "../common/type.h"

namespace CIL
{
   Field::Field():
      mName(),
      mType()
   {
   }

   Field::~Field()
   {
   }

    // - Get/set

   const String& Field::getName() const
   {
      return mName;
   }

   void Field::setName(const String& name)
   {
      mName = name;
   }

   const yasc::Type& Field::getType() const
   {
      return mType;
   }

   void Field::setType(const yasc::Type& type)
   {
      mType = type;
   }
}
