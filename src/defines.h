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

#include <assert.h>

#ifndef NULL
#define NULL 0
#endif

// Uncomment next line to enable auto disconnecting after timeout
//#define JENGINE_AUTODISCONNECT

// Uncomment the next line to disable inlining.
//#define JENGINE_INLINE

#define JENGINE_STATISTICS_LIMIT    250

#define JENGINE_MSG_HANDLED         0
#define JENGINE_MSG_UNHANDLED       1

#ifdef JENGINE_INLINE
#  define INLINE inline
#else
#  define INLINE
#endif

typedef unsigned int uint;
typedef void*        handle;

#define SWAP(type,x,y) { type temp = x; x = y; y = temp; }

#define MIN(x,y) ( x<y ? x : y )
#define MAX(x,y) ( x>y ? x : y )

#define IS_SET(container,flag)((container & flag) == flag)
#define SET_FLAG(container,flag)   container |= flag
#define CLEAR_FLAG(container,flag) container &= ~flag
#define SWAP_FLAG(container,flag) container ^= flag

// Debugging macros

/*
#define ASSERT(cond)          assert(cond);
#define ASSERT_MSG(cond,msg)  assert(cond && msg);
#define ASSERT_PTR(ptr)       assert(ptr != NULL);
#define PURE_VIRTUAL          assert(false && "Pure virtual!");
#define UNREACHABLE(msg)      assert(false && msg);
*/

#define ASSERT(cond)        ;
#define ASSERT_MSG(cond,msg) ;
#define ASSERT_PTR(ptr)     ;
#define PURE_VIRTUAL        ;
#define UNREACHABLE(msg)    ;


#endif
