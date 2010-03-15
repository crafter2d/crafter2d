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
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/GLee.h>
#include <GL/glu.h>

#include <iostream>
#include <string.h>

#include "console.h"
#include "cgengine.h"
#include "cgpath.h"
#include "codepath.h"
#include "opengl.h"
#include "vertexbufferarray.h"
#include "vertexbufferobject.h"
#include "shader.h"
#include "texture.h"
#include "particlesystem.h"

#ifdef WIN32
ShHandle OpenGL::vertexCompiler = 0;
#endif

/*!
    \fn OpenGL::initialize()
	 \brief Initializes the extensions neccessary for the engine to function correctly.
	 \return Nothing
 */
bool OpenGL::initialize ()
{
   Log& log = Console::getLog();
	GLint units;

   log << "Graphics card:\t\t" << (char*)glGetString(GL_VENDOR) << "\n";
   log << "OpenGL version:\t\t" << (char*)glGetString(GL_VERSION) << "\n";

   if ( !GLEE_VERSION_1_3 )
   {
      log << "OpenGL version 1.3 is required to run this application.\n";
      return false;
   }

	// make sure that there are enough texture units
	glGetIntegerv (GL_MAX_TEXTURE_UNITS, &units);
   if ( units < 2 )
   {
      log << "Videocard does not support 2 or more texture units.\n";
   }

   log << "Max textures:\t\t" << (int)units << "\n";
   int maxTextureSize = 0;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
   log << "Max texture size:\t" << maxTextureSize << "\n";

	// see if VBO's are supported
   if ( GLEE_ARB_vertex_buffer_object )
      log << "Using VBO extention for vertex storage.\n";
   else
      log << "Videocard does not support the VBO extension. Falling back on vertex arrays.\n";

   // create the context
	CGEngine& cg = CGEngine::getInstance();
	if ( !cg.initialize () )
      return false;

	return true;
}

/// \fn OpenGL::createVertexBuffer()
/// \brief Creates a vertex buffer for storage of the geometry. When Vertex Buffer Objects are supported, one will
/// be created. A default buffer will be created otherwise.
/// \return A pointer to the vertex buffer instance.
VertexBuffer* OpenGL::createVertexBuffer()
{
	VertexBuffer *vb = NULL;
	if ( GLEE_ARB_vertex_buffer_object )
		vb = new VertexBufferObject();
	else
		vb = new VertexBufferArray();

	return vb;
}

CodePath* OpenGL::createCodePath (CodePath::PathType pathtype)
{
   ASSERT(pathtype == CodePath::ECG || pathtype == CodePath::EGLSL);
	if ( pathtype == CodePath::ECG )
   {
      if ( CGEngine::getInstance().hasContext() )
		   return new CGPath();
   }
	else
   {
		if ( supportsGLSL() )
			return new ShaderPath();
		else
			return new ProgramPath();
	}

   return NULL;
}

bool OpenGL::supportsVertexShader(CodePath::PathType pathtype)
{
   ASSERT(pathtype == CodePath::ECG || pathtype == CodePath::EGLSL);
   if ( pathtype == CodePath::ECG )
      return CGEngine::getInstance().supportsVertexShader();
   else
      return GLEE_ARB_vertex_shader;
}

bool OpenGL::supportsFragmentShader(CodePath::PathType pathtype)
{
   ASSERT(pathtype == CodePath::ECG || pathtype == CodePath::EGLSL);
   if ( pathtype == CodePath::ECG )
      return CGEngine::getInstance().supportsFragmentShader();
   else
	   return GLEE_ARB_fragment_shader;
}

bool OpenGL::isPathSupported(CodePath::PathType pathtype)
{
   ASSERT(pathtype == CodePath::ECG || pathtype == CodePath::EGLSL);
   if ( pathtype == CodePath::ECG )
      return supportsCG();
   else
      return supportsGLSL();
}

bool OpenGL::supportsCG()
{
   return CGEngine::getInstance().hasContext();
}

bool OpenGL::supportsGLSL ()
{
	// GLSL is only supported if both vertex- and fragment shaders are available
	return (GLEE_ARB_shading_language_100 && GLEE_ARB_shader_objects && GLEE_ARB_vertex_shader);
}

#ifdef Win32
void GenerateResources(TBuiltInResource& resources)
{
    resources.maxLights = 32;
    resources.maxClipPlanes = 6;
    resources.maxTextureUnits = 32;
    resources.maxTextureCoords = 32;
    resources.maxVertexAttribs = 64;
    resources.maxVertexUniformComponents = 4096;
    resources.maxVaryingFloats = 64;
    resources.maxVertexTextureImageUnits = 32;
    resources.maxCombinedTextureImageUnits = 32;
    resources.maxTextureImageUnits = 32;
    resources.maxFragmentUniformComponents = 4096;
    resources.maxDrawBuffers = 32;
}
#endif

int OpenGL::glslToASM (char** code)
{
	int length = -1;
   /*
	const char* asmCode = NULL;
	TBuiltInResource resources;
	GenerateResources (resources);

	// construct a vertex compiler object
	vertexCompiler = ShConstructCompiler (EShLangVertex, EDebugOpAssembly);

	// try to compiler the given code
	if (ShCompile (vertexCompiler, code, 1, EShOptNone, &resources, EDebugOpAssembly)) {
		asmCode = ShGetInfoLog (vertexCompiler);
      Console::getInstance().print("OpenGL.glslToASM: converted GLSL to ASM.");

		length = (int)strlen (asmCode);
		delete[] (*code);
		(*code) = new char[length+1];
		strcpy ((*code), asmCode);
	}
	else {
		// report an error
      Console::getInstance().print("OpenGL.glslToASM: failed to compiler shader.");
      Console::getInstance().printf("%s", ShGetInfoLog (vertexCompiler));
	}

	ShDestruct(vertexCompiler);
   */
	return length;
}
