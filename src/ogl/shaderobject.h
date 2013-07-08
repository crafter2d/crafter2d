
#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <vector>
#include "shader.h"

namespace Graphics
{
   class ShaderUniformBuffer;

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

      GLuint      handle() const;
      bool        valid() const;

    // uniforms
      ShaderUniformBuffer* getUniformBuffer(const String& name) const;

      bool bindTexture(const Texture& uniform);

      GLint       getAttribute( const char* name ) const;
      GLint       getUniformLocation( const String& name ) const;

      void        setUniform1i( GLint index, int i );
	   void        setUniform3f( GLint index, float x, float y, float z );
      void        setUniform4f( GLint index, float x, float y, float z, float w );

   private:
      typedef std::vector<Shader*> Shaders;

    // data
      Shaders shaders;
      GLuint  program;
   };
};

#ifdef JENGINE_INLINE
#  include "shaderobject.inl"
#endif

#endif // SHADER_OBJECT_H
