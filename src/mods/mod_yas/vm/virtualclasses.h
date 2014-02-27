
#ifndef VIRTUAL_CLASSES_H
#define VIRTUAL_CLASSES_H

#include <vector>

class String;
class VirtualClass;

class VirtualClasses
{
public:
   VirtualClasses();

   VirtualClass& operator[](int index);

 // query
   bool contains(const VirtualClass& klass) const;
   int size() const;

 // operations
   void add(VirtualClass& klass);
   void add(VirtualClasses& classes);


private:
   typedef std::vector<VirtualClass*> Classes;

 // data
   Classes mClasses;
};

#endif // VIRTUAL_CLASSES_H
