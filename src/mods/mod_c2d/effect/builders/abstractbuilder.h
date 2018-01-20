
#ifndef ABSTRACT_BUILDER_H
#define ABSTRACT_BUILDER_H

#include <cstdint>

namespace Graphics
{
   class VertexLayout;
}

class ASTEffect;
class ASTTechnique;
class ASTFunction;
class ASTStruct;
class ASTType;
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

   virtual uint32_t toNativeType(const ASTType& type) = 0;

   void buildInputLayout(const ASTStruct& input, Graphics::VertexLayout& layout);

private:

   uint32_t getTypeSize(const ASTType& type);
};

#endif // ABSTRACT_BUILDER_H
