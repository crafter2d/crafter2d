
#include "compilestep.h"

#include "core/defines.h"

CompileStep::CompileStep()
{
}

// operations
   
bool CompileStep::step(ASTRoot& root)
{
   return performStep(root);
}

bool CompileStep::performStep(ASTRoot& root)
{
   UNREACHABLE("");
   return false;
}
