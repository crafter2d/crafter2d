
#ifndef COMPILE_INTERFACE_TABLE_H
#define COMPILE_INTERFACE_TABLE_H

#include <vector>

class ASTClass;

class CompileInterfaceTable
{
   typedef std::vector<ASTClass*> Interfaces;

public:
   CompileInterfaceTable();

 // maintenance
   void add(ASTClass& klass);


private:

   Interfaces mInterfaces;
   int        mIndex;
};

#endif // COMPILE_INTERFACE_TABLE_H
