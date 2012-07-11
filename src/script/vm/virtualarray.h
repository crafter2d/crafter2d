
#ifndef VIRTUAL_ARRAY_H_
#define VIRTUAL_ARRAY_H_

#include "script/script_base.h"

#include "script/gc/collectable.h"

class Variant;

class SCRIPT_API VirtualArray : public Collectable
{
public:
   VirtualArray();
   virtual ~VirtualArray();

   const Variant& operator[](int index) const;
         Variant& operator[](int index);

 // query
   const Variant& at(int index) const;
         Variant& at(int index);

   int size() const;

 // operations
   void addLevel(int size);
   void resize(int newsize);

   virtual void finalize(VirtualMachine& vm);

protected:
 // marking
   virtual void doMark();
   
private:
   Variant* mpArray;
   int      mSize;
};

#endif // VIRTUAL_ARRAY_H_
