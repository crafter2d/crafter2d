
#ifndef FUNCTION_RESOLVER_H
#define FUNCTION_RESOLVER_H

class ASTFunction;
class CompileContext;
class String;

class FunctionResolver
{
public:

   ASTFunction& resolve(CompileContext& context, const String& prototype);
};

#endif // FUNCTION_RESOLVER_H
