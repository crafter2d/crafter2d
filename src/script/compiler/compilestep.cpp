
#include "compilestep.h"

#include <boost/assert.hpp>

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
   BOOST_ASSERT(false);
   return false;
}
