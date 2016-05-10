
#ifndef VIRTUAL_FIELD_H
#define VIRTUAL_FIELD_H

#include "core/string/string.h"

#include "mod_yas/common/type.h"

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

 // data
   yasc::Type     mType;
   String         mName;
   VirtualClass*  mpClass;
   int            mIndex;
};

#endif // VIRTUAL_FIELD_H
