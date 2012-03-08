
#ifndef SCOPE_STACK_H_
#define SCOPE_STACK_H_

#include <string>
#include <deque>

class Scope;
class ScopeVariable;

class ScopeStack
{
   typedef std::deque<Scope*> Scopes;

public:
   ScopeStack();

 // query
   Scope& top();

 // operations
   void add(ScopeVariable* pvariable);

   void push(Scope* pscope);
   Scope* pop();

 // search
   ScopeVariable* find(const std::string& name);

private:
   Scopes mScopes;
};

#endif // SCOPE_STACK_H_
