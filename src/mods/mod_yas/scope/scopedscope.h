
#ifndef SCOPED_SCOPE_H_
#define SCOPED_SCOPE_H_

class Scope;
class ScopeStack;

class ScopedScope
{
public:
   ScopedScope(ScopeStack& stack);
   ~ScopedScope();

 // operators
   Scope& operator*();

private:
   ScopeStack& mStack;
};

#endif // SCOPED_SCOPE_H_
