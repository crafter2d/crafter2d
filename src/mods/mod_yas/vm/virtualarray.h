
#ifndef VIRTUAL_ARRAY_H_
#define VIRTUAL_ARRAY_H_

#include "mod_yas/script_base.h"

#include "mod_yas/gc/collectable.h"

class VirtualValue;

class VirtualArray : public Collectable
{
public:
   VirtualArray();
   virtual ~VirtualArray();

   const VirtualValue& operator[](int index) const;
         VirtualValue& operator[](int index);

 // query
   const VirtualValue& at(int index) const;
         VirtualValue& at(int index);

   int size() const;

 // operations
   void addLevel(int size);
   void resize(int newsize);

   virtual void finalize(VirtualMachine& vm);

protected:
 // marking
   virtual void doMark();
   
private:
   VirtualValue* mpArray;
   int           mSize;
};

#endif // VIRTUAL_ARRAY_H_
