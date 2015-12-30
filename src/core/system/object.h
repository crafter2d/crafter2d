
#ifndef OBJECT_H
#define OBJECT_H

class String;

class Object
{
public:
   virtual ~Object() {}

   virtual Object* clone() const = 0;
   virtual bool equals(const Object& that) const = 0;
   virtual String toString() const = 0;
};

#endif // OBJECT_H
