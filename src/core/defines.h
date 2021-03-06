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
#ifndef DEFINES_OF_JENGINE_H_
#define DEFINES_OF_JENGINE_H_

#include <cassert>
#include <exception>
#include <stdexcept>
#include <cstdint>

// Uncomment next line to enable auto disconnecting after timeout
//#define CRAFTER_AUTODISCONNECT

// Uncomment the next line to disable inlining.
//#define CRAFTER_INLINE

#define JENGINE_STATISTICS_LIMIT    250

#ifdef CRAFTER_INLINE
#  define INLINE inline
#else
#  define INLINE
#endif

#define SWAP(type,x,y) { type temp = x; x = y; y = temp; }

#ifndef MIN
#  define MIN(x,y) ( x<y ? x : y )
#  define MAX(x,y) ( x>y ? x : y )
#endif

#ifdef LINUX
#define stricmp strcasecmp
#endif

#define IS_SET(container,flag)((container & flag) == flag)
#define SET_FLAG(container,flag)   container |= flag
#define CLEAR_FLAG(container,flag) container &= ~flag
#define SWAP_FLAG(container,flag) container ^= flag

// Debugging macros

#define ASSERT(cond)          assert(cond);
#define ASSERT_MSG(cond,msg)  assert(cond && msg);
#define ASSERT_PTR(ptr)       assert(ptr != nullptr);
#define PURE_VIRTUAL          throw std::runtime_error("Pure virtual");
#define UNREACHABLE(msg)      throw std::runtime_error(msg);

// Warning prevention

#define C2D_UNUSED(var) (void)(var)

// MATH

#define PI 3.14159265358979323846f
#define DEG2RAD(deg)(deg * PI / 180)
#define RAD2DEG(rad)(rad * 180 / PI)

#endif
