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
#ifndef CODEPATH_H_
#define CODEPATH_H_

#include <string>
#include "shader.h"
#include "vertexprogram.h"

/*!
@author Jeroen Broekhuizen
\brief Pure virtual prototype for the different code paths used in this engine.

This class supplies the basic functions a code path must include when being implemented
in this engine. See the ShaderPath and ProgramPath for implementations currently used.
 */
class CodePath
{
public:
   enum PathType { ECG, EGLSL };

	/*!
        \fn CodePath::load(const char* vertex, const char* fragment)
	     \brief Load in the vertex and fragment shaders. This function should be called before any of the other functions.
		  \param vertex the filename of the vertex shader (either GLSL or ASM)
		  \param fragment the filename of the fragment shader (only GLSL)
	     \retval true if the vertex shader is loaded correctly
		  \retval false loading failed, consult the log file for possible reasons.
    */
	virtual bool load (const char* vertex, const char* fragment=NULL) = 0;

	/*!
        \fn CodePath::release()
	     \brief Releases the shaders.
    */
	virtual void release () = 0;

	/*!
        \fn CodePath::enable()
	     \brief Enable the shaders for use during rendering. When ready with the batch you should call disable
		  to stop using this shader with other geometry.
    */
	virtual void enable () const = 0;

	/*!
        \fn CodePath::disable()
	     \brief Disable the shaders. After this function is called the fixed pipeline will be used again.
    */
	virtual void disable () const = 0;

   virtual PathType getType() const = 0;

	/*!
        \fn CodePath::getUniformLocation(const char* name)
	     \brief Tries to retreive the location of a unifom variable in the shader.
		  \param name character string which should respresents an uniform variable in the shader
		  \returns a 0 or higher if it is found, -1 otherwise
    */
	virtual int getUniformLocation (const char* name) const = 0;

	/*!
        \fn CodePath::setUniform1i(int index, int val)
	     \brief Set the values of a one component uniform integer variable. The index of the uniform
		  variable should be retreived with the getUniformLocation function.
		  \param index the index of the uniform to set
		  \param val the value to set this uniform with
    */
	virtual void setUniform1i (int index, int val) = 0;

	/*!
        \fn CodePath::setUniform3f(int index, float x, float y, float z)
	     \brief Set the values of a two component uniform float variable. The index of the uniform
		  variable should be retreived with the getUniformLocation function.
		  \param index the index of the uniform to set
		  \param x,y,z the values to set this uniform with
    */
	virtual void setUniform3f (int index, float x, float y, float z) = 0;

	/*!
        \fn CodePath::setUniform4f(int index, float x, float y, float z, float w)
	     \brief Set the values of a four component uniform float variable. The index of the uniform
		  variable should be retreived with the getUniformLocation function.
		  \param index the index of the uniform to set
		  \param x,y,z,w the values to set this uniform with
    */
	virtual void setUniform4f (int index, float x, float y, float z, float w) = 0;
};

/*!
@author Jeroen Broekhuizen
\brief Code path that uses the shader objects as shader.

This code path uses the GLSL shader objects for shading the worlds. This path is used by default both
vertex and fragment shaders are supported on the target hardware.
 */
class ShaderPath: public CodePath
{
public:
	virtual bool     load(const char* vertex, const char* fragment);
	virtual void     release();

	virtual void     enable() const;
	virtual void     disable() const;

   virtual PathType getType() const;
	virtual int      getUniformLocation(const char* name) const;

	virtual void     setUniform1i(int index, int val);
	virtual void     setUniform3f(int index, float x, float y, float z);
	virtual void     setUniform4f(int index, float x, float y, float z, float w);

private:
	ShaderObject shader;
};

/*!
@author Jeroen Broekhuizen
\brief Code path that uses the VertexProgram class as shader.

This code path implements ARB vertex programs only. There is no support for fragment shaders in this
path (if fragments are supported on the target system the ShaderPath will be used instead).
 */
class ProgramPath: public CodePath
{
public:
   ProgramPath();
   virtual ~ProgramPath();

	virtual bool     load (const char* vertex, const char* fragment);
	virtual void     release ();

	virtual void     enable () const;
	virtual void     disable () const;

   virtual PathType getType() const;
	virtual int      getUniformLocation (const char* name) const;

	virtual void     setUniform1i (int index, int val);
	virtual void     setUniform3f (int index, float x, float y, float z);
	virtual void     setUniform4f (int index, float x, float y, float z, float w);

private:
	VertexProgram program;
};

#endif
