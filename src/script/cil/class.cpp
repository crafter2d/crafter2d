
#include "class.h"

namespace CIL
{

Class::Class():
   mFunctions()
{
}

// - Operations
   
void Class::addFunction(Function* pfunction)
{
   mFunctions.push_back(pfunction);
}

} // namespace CIL
