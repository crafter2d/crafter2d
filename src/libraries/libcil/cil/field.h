
#ifndef CIL_FIELD_H
#define CIL_FIELD_H

#include "core/string/string.h"

#include "../common/type.h"

namespace CIL
{
   class Class;

   class Field
   {
   public:
      Field();
      ~Field();

    // get/set
      const String& getName() const;
      void          setName(const String& name);

      const Type& getType() const;
      void        setType(const Type& type);

   private:

    // data
      String   mName;
      Type     mType;
   };
};

#endif // CIL_FIELD_H
