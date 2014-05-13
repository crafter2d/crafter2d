
#ifndef AST_TECHNIQUE_H
#define AST_TECHNIQUE_H

#include "astshader.h"

namespace Graphics
{
   class VertexLayout;
}

class ASTTechnique
{
public:

 // data
   String                  mName;
   Graphics::VertexLayout* mpLayout;
   ASTShader               mVertex;
   ASTShader               mGeometry;
   ASTShader               mPixel;
};

#endif // AST_TECHNIQUE_H
