
#ifndef CIL_GUARDS_H
#define CIL_GUARDS_H

#include <vector>

namespace CIL
{
   class Guard;

   class Guards
   {
      typedef std::vector<Guard*> GuardVector;

   public:
      Guards();
      ~Guards();
      
      const Guards& operator=(const Guards& that);
      const Guard&  operator[](int index) const;

    // query
      int size() const;

    // maintenance
      void add(Guard* pguard);
      void clear();

   private:
      GuardVector mGuards;
   };
}

#endif // CIL_GUARDS_H
