
#ifndef VIRTUAL_FIELD_H
#define VIRTUAL_FIELD_H

#include "core/string/string.h"

namespace yasc
{
   class Type;
}

class VirtualClass;

class VirtualField
{
public:
   VirtualField();
   VirtualField(const String& name, const yasc::Type& type);
   ~VirtualField();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   const VirtualClass& getClass() const;
   void                setClass(VirtualClass& klass);

   const yasc::Type& getType() const;
   void              setType(const yasc::Type& type);

   int  getIndex() const;
   void setIndex(int index);

private:
 // get/set
   void setType(yasc::Type* ptype);

 // data
   String         mName;
   VirtualClass*  mpClass;
   yasc::Type*    mpType;
   int            mIndex;
};

#endif // VIRTUAL_FIELD_H
