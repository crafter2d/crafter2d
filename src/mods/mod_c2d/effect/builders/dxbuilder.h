
#ifndef DX_BUILDER_H
#define DX_BUILDER_H

#include "abstractbuilder.h"

class ASTStruct;
class ASTStructEntry;
class String;

class DxBuilder : public AbstractBuilder
{
protected:
 // overrides
   virtual void buildVertexShader(const ASTEffect& effect, ASTTechnique& technique) override;
   virtual void buildPixelShader(const ASTEffect& effect, ASTTechnique& technique) override;

private:

 // operations
   String buildStructs(const ASTEffect& effect, const ASTFunction& function);
   String buildStruct(const ASTStruct& str);
   String buildStructEntry(const ASTStructEntry& entry);
   String buildFunction(const ASTFunction& function);
};

#endif // DX_BUILDER_H
