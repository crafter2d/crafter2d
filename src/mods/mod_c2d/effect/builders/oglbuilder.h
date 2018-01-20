
#ifndef OGL_BUILDER_H
#define OGL_BUILDER_H

#include "abstractbuilder.h"

class ASTEffect;
class ASTFunction;
class ASTStruct;
class String;

class OglBuilder : public AbstractBuilder
{
protected:
 // overrides
   virtual void buildVertexShader(const ASTEffect& effect, ASTTechnique& technique) override;
   virtual void buildGeometryShader(const ASTEffect& effect, ASTTechnique& technique) override;
   virtual void buildPixelShader(const ASTEffect& effect, ASTTechnique& technique) override;

   virtual uint32_t toNativeType(const ASTType& type) override;

private:

 // generations
   String buildTextures(const ASTEffect& effect, const ASTFunction& function);
   String buildVertexStructs(const ASTEffect& effect, ASTTechnique& technique, const ASTFunction& function);
   String buildGeometryStructs(const ASTEffect& effect, const ASTFunction& function);
   String buildPixelStructs(const ASTEffect& effect, const ASTFunction& function);
   String buildFunction(const ASTFunction& function);

   String buildInputStruct(const ASTStruct& str);
   String buildInputOutputStruct(const ASTStruct& str, const String& direction, bool asarray = false);
};

#endif // OGL_BUILDER_H
