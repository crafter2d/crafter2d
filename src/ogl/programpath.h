
#ifndef PROGRAM_PATH_H
#define PROGRAM_PATH_H

#include "core/graphics/codepath.h"

#include "vertexprogram.h"

namespace Graphics
{
   /*!
   @author Jeroen Broekhuizen
   \brief Code path that uses the VertexProgram class as shader.

   This code path implements ARB vertex programs only. There is no support for fragment shaders in this
   path (if fragments are supported on the target system the ShaderPath will be used instead).
    */
   class ProgramPath : public CodePath
   {
   public:
      ProgramPath();
      virtual ~ProgramPath();

	   virtual bool     load (const String& vertex, const String& fragment);
	   virtual void     release ();

	   virtual void     enable () const;
	   virtual void     disable () const;

      virtual PathType getType() const;
	   virtual int      getUniformLocation (const String& name) const;

      virtual void     setStateUniform(int index);
	   virtual void     setUniform1i (int index, int val);
	   virtual void     setUniform3f (int index, float x, float y, float z);
	   virtual void     setUniform4f (int index, float x, float y, float z, float w);

   private:
	   VertexProgram program;
   };
};

#endif // PROGRAM_PATH_H
