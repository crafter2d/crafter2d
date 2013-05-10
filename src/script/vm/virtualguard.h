
#ifndef VM_GUARD_H
#define VM_GUARD_H

namespace VM
{
   class Guard
   {
   public:
      Guard(): catchLabel(-1), finallyLabel(-1), endLabel(-1), finally(false) {}
      Guard(int jumpto, bool finally): catchLabel(-1), finallyLabel(-1), endLabel(-1), finally(finally) {}

      int  catchLabel;
      int  finallyLabel;
      int  endLabel;
      bool finally;
   };
}

#endif // VM_GUARD_H
