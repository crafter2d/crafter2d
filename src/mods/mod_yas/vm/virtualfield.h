
#ifndef VIRTUAL_FIELD_H
#define VIRTUAL_FIELD_H

#include "libcil/common/type.h"

#include "core/string/string.h"

class VirtualClass;

class VirtualField
{
public:
   VirtualField();
   VirtualField(const String& name, const CIL::Type& type);
   ~VirtualField();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   const VirtualClass& getClass() const;
   void                setClass(VirtualClass& klass);

   const CIL::Type& getType() const;
   void             setType(const CIL::Type& type);

   int  getIndex() const;
   void setIndex(int index);

private:

 // data
   CIL::Type      mType;
   String         mName;
   VirtualClass*  mpClass;
   int            mIndex;
};

#endif // VIRTUAL_FIELD_H
