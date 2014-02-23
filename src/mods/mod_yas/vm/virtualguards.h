
#ifndef VM_GUARDS_H
#define VM_GUARDS_H

#include <vector>

class VirtualGuard;

class VirtualGuards
{
   typedef std::vector<VirtualGuard*> GuardVector;

public:
   VirtualGuards();

   const VirtualGuard& operator[](int index) const;
         VirtualGuard& operator[](int index);

 // query
   int size() const;

 // maintenance
   void add(VirtualGuard* pguard);
   void updatePosition(int offset);

 // lookup
   const VirtualGuard* findGuard(int pos) const;

private:

 // data
   GuardVector mGuards;
};

#endif // VM_GUARDS_H
