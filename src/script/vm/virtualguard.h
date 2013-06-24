
#ifndef VM_GUARD_H
#define VM_GUARD_H

class VirtualGuard
{
public:
   static const int sStart = 0;
   static const int sCatch = 1;
   static const int sFinal = 2;
   static const int sEnd   = 3;


   void updatePosition(int offset) {
      locations[0] += offset;
      locations[1] += offset;
      locations[2] += offset;
      locations[3] += offset;
   }

   bool finalize;
   int  locations[4];
};

#endif // VM_GUARD_H
