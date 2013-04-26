
#ifndef CIL_FUNCTION_TABLE_H_
#define CIL_FUNCTION_TABLE_H_

#include <vector>

namespace CIL
{
   class Function;

   class FunctionTable
   {
      typedef std::vector<Function*> Functions;

   public:
      FunctionTable();

      const FunctionTable& operator=(const FunctionTable& that);

    // query
      int size() const;
      bool contains(const Function& function) const;

      const Function& operator[](int index) const;
            Function& operator[](int index);

    // operations
      void insert(Function& function);
      void insert(FunctionTable& table);

      void reindex();

   private:
      Functions mFunctions;
   };
}

#endif // CIL_FUNCTION_TABLE_H_
