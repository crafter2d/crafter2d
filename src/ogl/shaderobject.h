
#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <vector>
#include "shader.h"

namespace Graphics
{
   /**
   @author Jeroen Broekhuizen
   \brief The shader object class. It keeps track of the OpenGL program which can hold vertex- and fragment shaders. With this class
   you can use your shaders in the game.
   */
   class ShaderObject{
   public:
                  ShaderObject();
                  ~ShaderObject();

      bool        create();
	   void        release();

      void        addShader(Shader* shader);
      bool        link();

      void        enable() const;
      void        disable() const;

      GLhandleARB handle() const;
      bool        valid() const;

      GLint       getAttribute( const char* name ) const;
      GLint       getUniformLocation( const String& name ) const;

      void        setUniformStateMatrix(int index) const;
      void        setUniform1i( GLint index, int i );
	   void        setUniform3f( GLint index, float x, float y, float z );
      void        setUniform4f( GLint index, float x, float y, float z, float w );

   private:
      std::vector<Shader*> shaders;
      GLhandleARB program;
   };
};

#ifdef JENGINE_INLINE
#  include "shaderobject.inl"
#endif

#endif // SHADER_OBJECT_H
