
#ifndef SCOPE_H_
#define SCOPE_H_

#include <string>
#include <vector>

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
   ScopeVariable* find(const std::string& name);

private:
   Variables mVariables;
};

#endif // SCOPE_H_
