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
#ifndef _REFCOUNT_H_
#define _REFCOUNT_H_

// Low Latency Garbage Collection via Reference Counting, Photon, Mar 17, 2000
// http://www.gamedev.net/reference/articles/article1060.asp

class RefCount
{
public:
	virtual ~RefCount() {};

	/** Default constructor.  Initial reference count is 0,
      and will be incremented as soon as the object is
      pointed to. */
	RefCount(): refcnt(0) {};

	/** Add 1 to the reference count. */
	void addRef () { refcnt++; }

	/** Subtract 1 from the reference count.
      Returns true if the reference count has reached 0
      and the object should be deleted. */
	bool subRef () { return (--refcnt <= 0); }

private:
	long refcnt;
};

template<class T>
class RefPointer
{
public:
	RefPointer(T* p=0): ptr(p) { addRef(); }
	RefPointer(const RefPointer<T>& p): ptr(p.ptr) { addRef(); }
	~RefPointer() { subRef (); }

	/** Assignment operator. */
	RefPointer& operator= (const RefPointer& p) { return *this = p.ptr; }
	RefPointer& operator= (T* p) {
		if (p != ptr) {
			subRef ();
			ptr = p;
			addRef ();
		}
		return *this;
	}

   bool valid() const { return (ptr != 0); }

	/** Dereferencing operator. Provided to behave like the normal pointer. */
	T& operator* () const { return *ptr; }
	/** Member access operator. Provided to behave like the normal pointer. */
   T* operator-> () const { return ptr; }

	/** Conversion operators */
	operator T* () const { return ptr; }
	operator void*() { return this; } // necessary for the delete operator
	operator const T*() const { return ptr; }

private:

	void addRef () { if (ptr) ptr->addRef(); }
	void subRef () { 
		if (ptr) {
			if (ptr->subRef ()) {
				delete ptr;
				ptr = 0;
			}
		}
	}

	T* ptr;
};

#endif
