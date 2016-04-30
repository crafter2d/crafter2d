
#ifndef VIRTUAL_STRING_CACHE_H
#define VIRTUAL_STRING_CACHE_H

#include "mod_yas/script_base.h"

#include "core/string/string.h"

#include "mod_yas/gc/collectable.h"

class VirtualString : public Collectable
{
public:
            VirtualString();
   explicit VirtualString(const String& value);
            VirtualString(const VirtualString& that);
   virtual ~VirtualString();

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
};

#endif // VIRTUAL_STRING_CACHE_H
