#ifndef EFFECT_H_
#define EFFECT_H_

#include <string>

#include "resources.h"

class CodePath;
class Texture;
class TiXmlElement;

/*!
@author Jeroen Broekhuizen
\brief Implements an effect for a collection of triangles.

This class implements the an effect for use during rendering of objects. It keeps track
of textures (one diffuse map and extra textures for other stages), shaders and eventually
texture combiners. Either GLSL or vertex programs are used depending on the target hardware
(if GLSL is not supported the GLSL code is automatically converted to vertex program ASM
code).
 */
class Effect
{
	struct TexStage {
		TexturePtr tex;
      std::string uniform;
		int index;
		GLint combiner;
		GLint source0;
		GLint source1;
	};
	
public:
	                  Effect();
	                  ~Effect();
	
	bool              load(const char* file);
	void              destroy();
	
	const Texture&    resolveTexture(const char* uniform) const;
   const Texture*    findTexture(const char* uniform) const;

         bool        hasPath() const;
   const CodePath&   getPath() const;
         void        setPath(CodePath* path);
	
	void              enable () const;
	void              disable () const;
	
protected:
	bool              processTextures( const TiXmlElement* effect );
	bool              processCode( const TiXmlElement* effect );
	bool              processCombiners( const TiXmlElement* shader_part );
	bool              postprocessTextures();
	
	GLint             getCombinerValue( const char* str );
	GLint             getSourceValue( const char* str );
	
private:
	CodePath* path;
   bool useCombiners;
   std::vector<TexStage> stages;
   std::string name;
};

#ifdef JENGINE_INLINE
#  include "effect.inl"
#endif

#endif
