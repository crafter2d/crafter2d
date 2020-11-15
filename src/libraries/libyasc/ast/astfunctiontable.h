
#ifndef FUNCTION_TABLE_H_
#define FUNCTION_TABLE_H_

#include <vector>

class ASTFunction;
class ASTFunctionMap;

class ASTFunctionTable
{
   typedef std::vector<ASTFunction*> Functions;

public:
   ASTFunctionTable();

   const ASTFunctionTable& operator=(const ASTFunctionTable& that);

 // query
   int size() const;
   bool contains(const ASTFunction& function) const;

   const ASTFunction& operator[](int index) const;
         ASTFunction& operator[](int index);

 // operations
   void build(ASTFunctionMap& functions);

   void set(int index, ASTFunction& function);
   void insert(ASTFunction& function);
   void insert(ASTFunctionTable& table);

   void reindex(int base = 0);

private:
   Functions mFunctions;
};

#endif // FUNCTION_TABLE_H_
