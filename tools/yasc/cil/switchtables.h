
#ifndef CIL_SWITCHTABLES_H
#define CIL_SWITCHTABLES_H

#include <vector>

namespace CIL
{
   class SwitchTable;

   class SwitchTables
   {
      typedef std::vector<SwitchTable*> Tables;

   public:

      const SwitchTables& operator=(const SwitchTables& that);

      const SwitchTable& operator[](int index) const;
            SwitchTable& operator[](int index);

    // query
      int size() const;

    // maintenance
      int add(SwitchTable* ptable);
      void clear();

   private:
      Tables mTables;
   };
}

#endif // CIL_SWITCHTABLES_H
