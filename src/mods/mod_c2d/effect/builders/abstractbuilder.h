
#ifndef ABSTRACT_BUILDER_H
#define ABSTRACT_BUILDER_H

namespace Graphics
{
   class VertexLayout;
}

class ASTEffect;
class ASTTechnique;
class ASTFunction;
class ASTStruct;
class DataStream;

class AbstractBuilder
{
public:
   AbstractBuilder();
   virtual ~AbstractBuilder();

   bool build(ASTEffect& effect);

protected:

   virtual void buildVertexShader(const ASTEffect& effect, ASTTechnique& technique) = 0;
   virtual void buildGeometryShader(const ASTEffect& effect, ASTTechnique& technique) = 0;
   virtual void buildPixelShader(const ASTEffect& effect, ASTTechnique& technique) = 0;

   Graphics::VertexLayout* buildInputLayout(const ASTStruct& input);
};

#endif // ABSTRACT_BUILDER_H
