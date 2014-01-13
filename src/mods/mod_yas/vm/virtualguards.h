
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

   // maintenance
   void add(VirtualGuard* pguard);
   void updatePosition(int offset);

   // lookup
   const VirtualGuard* findGuard(int pos) const;

private:
   GuardVector mGuards;
};

#endif // VM_GUARDS_H
