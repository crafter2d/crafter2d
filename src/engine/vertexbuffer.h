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
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

struct VertexBufferDesc {
	int index;
	int size;
	int pos;
	int flags;
	float* pointer;
};

class Effect;

/**
@author Jeroen Broekhuizen
\brief A vertex buffer usable for efficient rendering of layers and objects.

A vertex buffer is an object in memory in which you can store the vertex attribute values of your layer or object and is highly
recommended to use. Using these buffers is an efficient way to render the objects to the screen. Don't allocate a vertex buffer yourself, but use the OpenGL::createVertexBuffer() function. It will determine what kind of vertex buffer is supported on the machine and returns a buffer automatically. Then call the create function to initialize it.
*/
class VertexBuffer
{
public:
   enum Usage {
      eStatic    = 1,
      eDynamic   = 2,
      eStream    = 4,
      eRead      = 8,
      eWriteOnly = 16
   };

   enum VertexFormat {
      eXY             = 1,
      eXYZ            = 2,
      eXYZW           = 4,
      eNormal         = 8,
      eDiffuse        = 16,
      eSpecular       = 32,
      eTex0           = 64,
      eTex1           = 128,
      eTex2           = 256,
      eTex3           = 512,
      eTex4           = 1024,
      eTangent        = 2048,
      eBinormal       = 4096,

      eMax            = eBinormal
   };

   VertexBuffer();
   virtual ~VertexBuffer();

	 /*!
	     \fn VertexBuffer::create (int length, int usage, int fvf)
		  \brief Creates the actuall vertex buffer object you can fill later on.
	 	  \param[in] length the number of items that should fit in this buffer
	     \param[in] usage  the usage of this buffer (static, dynamic, etc)
	     \param[in] fvf    the vertex format specification (position, texture coordinates, etc)
	     \retval true      the object is created successfully
	     \retval false     the usage flag or VBO's are invalid or the system ran out of memory
	  */
    virtual bool create(const Effect& effect, int length, int usage, int fvf) = 0;

	 /*!
	     \fn VertexBuffer::release()
	     \brief Releases the vertex buffer object and internal memory.
	     \return Nothing
	  */
	 virtual void release() = 0;

	 /*!
        \fn VertexBuffer::lock(int fvf)
	     \brief Locks the specified vertex attributes so they can be changed with setAt(). Only the setAt function is allowed untill you call unlock().
	     \param[in] fvf  specify the vertex attributes which should be locked.
	     \retval true the vertex attributes are locked successfully
	     \retval false could not map a vertex attribute
     */
	 virtual float* lock(int fvf) = 0;

	 /*!
        \fn VertexBuffer::unlock()
	     \brief Unlocks all locked vertex attributes. After calling this function you should not call setAt til you called lock again.
	     \return Nothing
     */
    virtual void unlock() = 0;

	/*!
        \fn VertexBuffer::enable()
	     \brief Enables the vertex buffer for use during rendering. Call disable to stop using this buffer.
	     \return Nothing
     */
    virtual void enable() = 0;

	 /*!
        \fn VertexBuffer::disable()
        \brief Disables the vertex buffer for rendering. Until enabled again data from this buffer will not be rendered.
	     \return Nothing
     */
    virtual void disable() = 0;

protected:
 // query
   int getFieldCount(int fvf);
   int getFVFSize(int fvf);

   // operation
   void fillDescription (int fvf);

   VertexBufferDesc* mpFields;
   int               mFieldCount;
   int               mStride;
};

#endif
