
#ifndef AST_TECHNIQUE_H
#define AST_TECHNIQUE_H

#include "astshader.h"

#include "core/graphics/vertexlayout.h"

class ASTTechnique
{
public:

 // data
   String                  mName;
   Graphics::VertexLayout  mLayout;
   ASTShader               mVertex;
   ASTShader               mGeometry;
   ASTShader               mPixel;
};

#endif // AST_TECHNIQUE_H
