/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "effect.h"
#ifndef JENGINE_INLINE
#  include "effect.inl"
#endif

#include <GL/GLee.h>

#include "core/vfs/file.h"
#include "core/log/log.h"
#include "core/defines.h"

#include "codepath.h"
#include "opengl.h"
#include "texture.h"
#include "tinyxml.h"

/*!
    \fn Effect::Effect(void)
	 \brief initializes internal member variables (by default texture combiners are not used).
 */
Effect::Effect():
   name(),
   stages(),
   mCodePath(NULL),
   useCombiners(false)
{
}

/*!
    \fn Effect::~Effect(void)
	 \brief releases internal member variables.
 */
Effect::~Effect()
{
	destroy();
}

/*!
    \fn Effect::load(const char* file)
	 \brief Loads an effect file and prepairs the OpenGL textures and shaders. It also
	 sets up pointers to the uniform variables.
	 \retval true the effect file is loaded correctly.
	 \retval false an error occured (look in the log file for a message).
 */
bool Effect::load(const std::string& file)
{
	Log& log = Log::getInstance();

   std::string path = File::extractPath(file);

   TiXmlDocument doc(file);
	if ( !doc.LoadFile() )
   {
      log.error("Effect.load: can not load '%s'", file);
		return false;
	}

	// get the root element from the file
	TiXmlElement* effect = (TiXmlElement*)doc.FirstChild("effect");
	if ( effect == NULL )
   {
		log.error("Effect.load: %s is not an effect file.", file);
		return false;
	}

   // save the name (if supplied)
   name = effect->Attribute ("name");

	// try to load in the textures
	if (!processTextures (effect))
		return false;

	// process the vertex program
	if (!processCode (effect, path))
	    return false;

	// find the uniform indices of the texture
	postprocessTextures();
	return true;
}

/*!
    \fn Effect::destroy()
	 \brief Releases the codepath and the data for the stages. After calling this function you must
	 call load before enabling it again.
 */
void Effect::destroy ()
{
   if( mCodePath != NULL )
   {
		// release the path
		mCodePath->release ();
		delete mCodePath;
		mCodePath = NULL;
	}

	stages.clear ();
}

/*!
    \fn Effect::processTextures( const TiXmlElement* effect )
	 \brief Creates/loads in the textures and their uniform names from the effect file for the
	 various stages neccessary for this effect.
	 \returns true when no errors are detected, false otherwise.
 */
bool Effect::processTextures( const TiXmlElement* effect )
{
   TiXmlElement* texture = (TiXmlElement*)effect->FirstChild("texture");
   while (texture)
   {
      TexStage stage;

		// process this texture
    	stage.uniform = texture->Attribute("uniform");
    	TiXmlText* file = (TiXmlText*)texture->FirstChild();
		const char* type = texture->Attribute("type");
		if (type && strcmp (type, "normcube") == 0)
      {
			// make it a normalizing cube map
         stage.tex = new Texture();
			stage.tex->createNormalizingCube();
		}
		else
      {
			// must be a normal texture
         stage.tex = ResourceManager::getInstance().loadTexture(file->Value());
         if ( !(stage.tex) )
         {
            Log::getInstance().error("Effect.processTextures: could not load texture %s", file->Value());
				return false;
			}
		}

      stages.push_back(stage);
		stage.tex->setStage(static_cast<int>(stages.size()-1));

		// now iterate over the rest of the textures
		texture = (TiXmlElement*)effect->IterateChildren ("texture", texture);
	}

	// we need at least one texture
	return (stages.size() > 0);
}

/*!
    \fn Effect::postprocessTextures()
	 \brief When GLSL is supported the uniform location of the textures are queried, otherwise
	 nothing is done.
	 \returns true when no errors are detected, false otherwise.
 */
bool Effect::postprocessTextures ()
{
   // get the uniform locations of the textures in the fragment shader
   for ( Stages::size_type s = 0; s < stages.size(); ++s)
   {
      stages[s].index = getPath().getUniformLocation(stages[s].uniform.c_str());
      if (stages[s].index == -1) 
      {
         Log::getInstance().error("Can not find %s", stages[s].uniform.c_str());
         return false;
      }
   }

	return true;
}

/*!
    \fn Effect::processCode( const TiXmlElement* effect )
	 \brief Loads in the shaders (if any) from the file and creates the code path. In case GLSL isn't
	 supported it first checks the existence of a vertex program. If that doesn't exist the GLSL counter
	 part is loaded and automatically converted.
	 \returns true when no errors are detected, false otherwise.
 */
bool Effect::processCode(const TiXmlElement* effect, const std::string& path)
{
   const char* vertex = NULL, *fragment = NULL;

   TiXmlElement* code_part = (TiXmlElement*)effect->FirstChild("code");
   if ( code_part == NULL )
   {
      Log::getInstance().error("Effect.processCode: effect file doesn't contain a code block!");
		return false;
   }

   // get the code type
   CodePath::PathType pathtype = CodePath::ECG;
   const char* ptype = code_part->Attribute("type");
   if ( strcmp(ptype, "glsl") == 0 || strcmp(ptype, "GLSL") == 0 )
      pathtype = CodePath::EGLSL;

   // load the vertex shader
   TiXmlElement* shader_part = (TiXmlElement*)code_part->FirstChild("vertex");
   if ( shader_part != NULL && OpenGL::supportsVertexShader(pathtype) )
   {
      TiXmlText* psource = (TiXmlText*)shader_part->FirstChild();
      ASSERT_PTR(psource);

	   vertex = psource->Value ();
   }

   // load the fragment shader
   shader_part = (TiXmlElement*)code_part->FirstChild("fragment");
   if ( shader_part && OpenGL::supportsFragmentShader(pathtype) )
   {
	   // see if there is a GLSL shader available
      TiXmlText* psource = (TiXmlText*)shader_part->FirstChild();
      ASSERT_PTR(psource);

	   fragment = psource->Value ();
   }

   // files should be in same directory
   std::string vertexfile   = path + vertex;
   std::string fragmentfile = path + fragment;

   // now load the codepath
   mCodePath = OpenGL::createCodePath(pathtype);
   if ( mCodePath == NULL || !mCodePath->load(vertexfile, fragmentfile) )
       return false;

   // is no fragment shader or not supported, look for combiners
   if ( fragment == NULL || !OpenGL::supportsFragmentShader(pathtype) )
   {
      useCombiners = true;

      if ( stages.size() > 0 && !processCombiners(code_part) )
         return false;
   }

	return true;
}

/*!
    \fn Effect::processCombiners(TiXmlElement* effect)
	 \brief Loads in the texture combiner setup per stage.
	 \returns true when no errors are detected, false otherwise.
 */
bool Effect::processCombiners( const TiXmlElement* shader_part )
{
	TiXmlElement* combiner_part = (TiXmlElement*)shader_part->FirstChild("combiner");
   if ( combiner_part == NULL )
   {
      Log::getInstance().error("Effect.processCombiners: could not find combiner for effect.");
      return true;
   }

	char stagebuf[100];
	sprintf (stagebuf, "stage0");
	TiXmlElement* stage_part = (TiXmlElement*)combiner_part->FirstChild(stagebuf);
	for (int i = 0; i < stages.size(); ++i)
   {
		if ( stage_part == NULL )
      {
			Log::getInstance().error("Effect.processCombiners: there is no combiner for stage %d", i);
			return false;
		}

		// get the text with the combiner, source0 and source1
		stages[i].combiner = getCombinerValue (stage_part->Attribute("combiner"));
		stages[i].source0 = getSourceValue (stage_part->Attribute("source0"));
		stages[i].source1 = getSourceValue (stage_part->Attribute("source1"));

		// get next stage
		sprintf (stagebuf, "stage%d", i+1);
		stage_part = (TiXmlElement*)combiner_part->IterateChildren (stagebuf, stage_part);
	}

	return true;
}

void strupr(char *dest, const char *src) {
	while (*src!='\0') {
		if (*src>=97 && *src<=122)
			*dest = *src-32;
  		else
			*dest = *src;
		src++;
		dest++;
	}
	dest = '\0';
}

/*!
    \fn Effect::getCombinerValue(const char* str)
	 \brief Converts the string value to a texture combiner OpenGL enum value.
	 \returns the corresponding value if str is known, -1 if an error occured or GL_MODULATE in case str = NULL.
 */
GLint Effect::getCombinerValue (const char* str)
{
	if (str == NULL)
	    return GL_MODULATE;

	char buf[256];
	memset (buf, 0, 256);
	strupr (buf, str);

	// now check for the combiner names
	if (strcmp (buf, "MODULATE") == 0)
	    return GL_MODULATE;
	else if (strcmp (buf, "REPLACE") == 0)
	    return GL_REPLACE;
	else if (strcmp (buf, "DOT3") == 0)
	    return GL_DOT3_RGB;
	else if (strcmp (buf, "ADD") == 0)
	    return GL_ADD;
	return -1;
}

/*!
    \fn Effect::getSourceValue(const char* str)
	 \brief Converts the string value to a texture source OpenGL enum value.
	 \returns the corresponding value if str is known, -1 if an error occured or 0 in case str = NULL (means not to set sources).
 */
GLint Effect::getSourceValue (const char* str)
{
	if (str == NULL)
		return 0;

	// there are no such long sources
	if (strlen (str) > 20)
		return -1;

	// check the sources
	char buf[20];
	memset (buf, 0, 20);
	strupr (buf, str);

	// now check for the combiner names
	if (strcmp (buf, "PREVIOUS") == 0)
	    return GL_PREVIOUS;
	else if (strcmp (buf, "TEXTURE") == 0)
	    return GL_TEXTURE;
	else if (strcmp (buf, "CONSTANT") == 0)
	    return GL_CONSTANT;
	return -1;
}

/*!
    \fn Effect::getTexture(const char* uniform)
	 \brief Looks up the uniform string in the stages of this effect. If found the texture object is returned.
	 \returns a texture object if uniform is known, NULL otherwise.
 */
const Texture& Effect::resolveTexture (const char* uniform) const
{
   const Texture* ptexture = findTexture(uniform);
   ASSERT_MSG(ptexture, "Can not find the texture.");
   return *ptexture;
}

const Texture* Effect::findTexture(const char* uniform) const
{
   for ( Stages::size_type s = 0; s < stages.size(); ++s )
   {
      const TexStage& stage = stages[s];
      if ( strcmp (uniform, stage.uniform.c_str()) == 0 )
         return stage.tex;
   }

   return NULL;
}

/*!
    \fn Effect::enable()
	 \brief Enables the textures, shaders and if neccessary the texture combiners.
 */
void Effect::enable () const
{
   getPath().enable();

   for ( Stages::size_type s = 0; s < stages.size(); ++s )
   {
	   const TexStage& stage = stages[s];
	   stage.tex->enable ();
	   if ( useCombiners )
      {
		   // enable the combiners
		   glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		   glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, stage.combiner);
		   if (stage.source0 > 0)
         {
			   glTexEnvf (GL_TEXTURE_ENV, GL_SOURCE0_RGB, stage.source0);
			   glTexEnvf (GL_TEXTURE_ENV, GL_SOURCE1_RGB, stage.source1);
		   }
	   }
	   else
      {
		   // set textures for GLSL language
         if ( OpenGL::supportsFragmentShader(mCodePath->getType()) )
            mCodePath->setUniform1i (stage.index, s);
	   }
   }
}

/*!
    \fn Effect::disable()
	 \brief Disables the textures, shaders and eventually resets the texture combiners.
 */
void Effect::disable () const
{
   getPath().disable();

   // disable the textures and reset their environment
   for ( Stages::size_type s = 0; s < stages.size(); ++s )
   {
      const TexStage& stage = stages[s];
      stage.tex->disable ();

      if ( useCombiners )
      {
         glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      }
   }
}
