
#ifndef VM_GUARDS_H
#define VM_GUARDS_H

#include <vector>

#include "virtualguard.h"

class VirtualGuards
{
   using Guards = std::vector<VirtualGuard>;

public:
   VirtualGuards();

   const VirtualGuard& operator[](int index) const {
      return mGuards[index];
   }

   VirtualGuard& operator[](int index) {
      return mGuards[index];
   }

 // query
   int size() const {
      return mGuards.size();
   }

 // maintenance
   void add(const VirtualGuard& guard) {
      mGuards.push_back(guard);
   }

   void updatePosition(int offset);

 // lookup
   const VirtualGuard* findGuard(int pos) const;

private:

 // data
   Guards mGuards;
};

#endif // VM_GUARDS_H
