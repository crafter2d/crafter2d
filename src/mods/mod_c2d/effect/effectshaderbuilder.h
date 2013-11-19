
#ifndef EFFECT_SHADER_BUILDER_H
#define EFFECT_SHADER_BUILDER_H

#include <vector>

namespace Graphics
{
   class VertexLayout;
}

class ASTEffect;
class ASTFunction;
class ASTStruct;
class ASTStructEntry;
class ASTTechnique;
class String;

class EffectShaderBuilder
{
public:

   void build(ASTEffect& effect);

protected:

   void buildVertexShader(const ASTEffect& effect, ASTTechnique& tehnique);
   void buildPixelShader(const ASTEffect& effect, ASTTechnique& technique);

   String buildStructs(const ASTEffect& effect, const ASTFunction& function);
   String buildStruct(const ASTStruct& str);
   String buildStructEntry(const ASTStructEntry& entry);
   String buildFunction(const ASTFunction& function);
   
   Graphics::VertexLayout* buildInputLayout(const ASTFunction& function);
};

#endif // EFFECT_SHADER_BUILDER_H
