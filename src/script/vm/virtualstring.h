
#ifndef VIRTUAL_STRING_CACHE_H
#define VIRTUAL_STRING_CACHE_H

#include "script/script_base.h"

#include "core/string/string.h"

#include "script/gc/collectable.h"

class SCRIPT_API VirtualString : public Collectable
{
public:
            VirtualString();
   explicit VirtualString(const String& value);
            VirtualString(const VirtualString& that);

 // operators
   bool operator==(const VirtualString& that) const;
   bool operator<=(const VirtualString& that) const;
   bool operator<(const VirtualString& that) const;
   bool operator>(const VirtualString& that) const;
   bool operator>=(const VirtualString& that) const;

 // get/set
   const String& getString() const;
   void          setString(const String& value);

 // overloads
   virtual void finalize(VirtualMachine& machine);

protected:
 // overloads
   virtual void doMark();

private:
   String mString;
   int    mHash;
};

#endif // VIRTUAL_STRING_CACHE_H
