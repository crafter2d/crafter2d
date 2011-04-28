
#ifndef VIRTUAL_ARRAY_OBJECT_H_
#define VIRTUAL_ARRAY_OBJECT_H_

class Variant;

class VirtualArrayObject
{
public:
   VirtualArrayObject();
   virtual ~VirtualArrayObject();

   const Variant& operator[](int index) const;
         Variant& operator[](int index);

 // query
   const Variant& at(int index) const;
         Variant& at(int index);

   int size() const;

 // operations
   void addLevel(int size);

 // downcast
   virtual VirtualArrayObject& asArray();

private:
   Variant* mpArray;
   int      mSize;
};

#endif // VIRTUAL_ARRAY_OBJECT_H_
