
#ifndef SCOPE_H_
#define SCOPE_H_

#include <vector>

class String;
class ScopeVariable;

class Scope
{
   typedef std::vector<ScopeVariable*> Variables;

public:
   Scope();
   ~Scope();

 // operations
   void add(ScopeVariable* pvariable);
   void clear();

 // search
   ScopeVariable* find(const String& name);

private:
   Variables mVariables;
};

#endif // SCOPE_H_
