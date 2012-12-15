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

#include "core/core_base.h"

class String;

namespace Graphics
{
   /*!
   @author Jeroen Broekhuizen
   \brief Pure virtual prototype for the different code paths used in this engine.

   This class supplies the basic functions a code path must include when being implemented
   in this engine. See the ShaderPath and ProgramPath for implementations currently used.
    */
   class CORE_API CodePath
   {
   public:
      enum PathType { ECG, EGLSL };

	   /*!
           \fn CodePath::load(const String& vertex, const String& fragment)
	        \brief Load in the vertex and fragment shaders. This function should be called before any of the other functions.
		     \param vertex the filename of the vertex shader (either GLSL or ASM)
		     \param fragment the filename of the fragment shader (only GLSL)
	        \retval true if the vertex shader is loaded correctly
		     \retval false loading failed, consult the log file for possible reasons.
       */
	   virtual bool load(const String& vertex, const String& fragment) = 0;

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

      /*!
           \fn CodePath::getType()
	        \brief Returns the type of code path this object represents.
       */
      virtual PathType getType() const = 0;

	   /*!
           \fn CodePath::getUniformLocation(const String& name)
	        \brief Tries to retreive the location of a unifom variable in the shader.
		     \param name character string which should respresents an uniform variable in the shader
		     \returns a 0 or higher if it is found, -1 otherwise
       */
	   virtual int getUniformLocation (const String& name) const = 0;

      virtual void setStateUniform(int index) = 0;

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
};

#endif
