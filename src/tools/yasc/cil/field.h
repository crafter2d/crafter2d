
#ifndef CIL_FIELD_H
#define CIL_FIELD_H

#include "core/string/string.h"

namespace yasc
{
   class Type;
}

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

      const yasc::Type& getType() const;
      void              setType(yasc::Type* ptype);

   private:

    // data
      String      mName;
      yasc::Type* mpType; // owned
   };
};

#endif // CIL_FIELD_H
