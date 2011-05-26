
#ifndef FUNCTION_TABLE_H_
#define FUNCTION_TABLE_H_

#include <vector>

class ASTFunction;

class FunctionTable
{
   typedef std::vector<ASTFunction*> Functions;

public:
   FunctionTable();

   const FunctionTable& operator=(const FunctionTable& that);

 // query
   int size() const;
   bool contains(const ASTFunction& function) const;

   const ASTFunction& operator[](int index) const;
         ASTFunction& operator[](int index);

 // operations
   void insert(ASTFunction& function);
   void insert(FunctionTable& table);

   void reindex();

private:
   Functions mFunctions;
};

#endif // FUNCTION_TABLE_H_
