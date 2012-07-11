/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#ifndef VIRTUAL_OBJECT_H_
#define VIRTUAL_OBJECT_H_

#include "script/script_base.h"

#include "script/gc/collectable.h"

class Variant;
class VirtualClass;
class VirtualMachine;

class SCRIPT_API VirtualObject : public Collectable
{
public:
   VirtualObject();
   ~VirtualObject();
   
 // get/set
   bool  hasNativeObject() const;
   void* getNativeObject();
   void* useNativeObject();
   void  setNativeObject(void* pobject);
      
   bool isOwner() const;
   void setOwner(bool owned);

   bool isMarked() const;
   void setMarked(bool marked);

 // query
   const VirtualClass& getClass() const;
   void                setClass(const VirtualClass& definition);

   int getMemberCount() const;

 // operation
   void initialize(int variables);
   Variant& getMember(int index);
   void setMember(int index, const Variant& value);
   virtual void finalize(VirtualMachine& vm);

protected:
 // marking
   virtual void doMark();

private:

 // members
   const VirtualClass*           mpClass;
   void*                         mpNativeObject;
   Variant*                      mpMembers;
   int                           mMemberCount;
   bool                          mOwnsNative;
   bool                          mMarked;
};

#endif // VIRTUAL_OBJECT_H_
