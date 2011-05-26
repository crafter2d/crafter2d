
#include "compilestep.h"

#include "core/defines.h"

CompileStep::CompileStep()
{
}

// operations
   
bool CompileStep::step(ASTNode& node)
{
   return performStep(node);
}

bool CompileStep::performStep(ASTNode& node)
{
   UNREACHABLE("");
   return false;
}
