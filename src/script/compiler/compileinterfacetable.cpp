
#include "compileinterfacetable.h"

#include "script/ast/astclass.h"
#include "script/ast/astfunctiontable.h"

CompileInterfaceTable::CompileInterfaceTable():
   mInterfaces(),
   mIndex(0)
{
}

// - Maintenance
   
void CompileInterfaceTable::add(ASTClass& klass)
{
   mInterfaces.push_back(&klass);

   ASTFunctionTable& functions = klass.getFunctionTable();
   functions.reindex(mIndex);

   mIndex += functions.size();
}
