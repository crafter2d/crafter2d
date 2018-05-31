
#include "field.h"

#include "core/defines.h"

#include "../common/type.h"

namespace CIL
{
   Field::Field():
      mName(),
      mpType(nullptr)
   {
   }

   Field::~Field()
   {
      setType(nullptr);
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
      ASSERT_PTR(mpType);
      return *mpType;
   }

   void Field::setType(yasc::Type* ptype)
   {
      delete mpType;
      mpType = ptype;
   }
}
