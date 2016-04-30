
#ifndef VIRTUAL_STACK_H
#define VIRTUAL_STACK_H

#include "mod_yas/script_base.h"

#include <vector>

#include "virtualvalue.h"

class String;
class VirtualArray;
class VirtualString;

class VirtualStack
{
public:
   explicit VirtualStack(int initialsize = 32);
            ~VirtualStack();

   const VirtualValue& operator[](int index) const;
         VirtualValue& operator[](int index);

 // stack operations
   void push(int count, VirtualValue* pvalues);
   void push(const VirtualValue& value);
   void pushInt(int value);
   void pushReal(float value);
   void pushBool(bool value);
   void pushChar(UChar value);
   void pushString(VirtualString& value);
   void pushObject(VirtualObject& object);
   void pushArray(VirtualArray& array);

   VirtualValue              pop();
   void                 pop(int count);
   int                  popInt();
   float                popReal();
   bool                 popBool();
   UChar                popChar();
   const String&        popString();
   VirtualObject&       popObject();
   VirtualArray&        popArray();

   void insert(int index, const VirtualValue& value);
   void setSize(int size);

 // query
   VirtualValue& back();

   int size() const;

 // operations
   void mark();

private:
   typedef std::vector<VirtualValue> Stack;

 // operations
   VirtualValue& top();
   void          ensureFits(int amount);
   void          fill(int from);

   Stack mStack;
   int   mSize;
};

#endif // VIRTUAL_STACK_H
