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
#ifndef NET_DEFINES_H_
#define NET_DEFINES_H_

enum NetObjectId
{
   NetObjectId             = 1,
   
   NetEventId              = 2,
   JoinEventId             = 3,
   DisconnectEventId       = 4,
   InputEventId            = 5,
   NewObjectEventId        = 6,
   
   SceneObjectId           = 101,
   ObjectId                = 102,
   AnimObjectId            = 103,
   CreatureId              = 104,
   WorldId                 = 105,
   ParticleSystemId        = 106,
   
   LaserId                 = 501
};

/// Add this macro to the class definition which should be able to replicate
#define DEFINE_REPLICATABLE(className)                                     \
   virtual void initInfo(RuntimeInfo* info) const;                         \
   virtual RuntimeInfo* getRuntimeInfo() const;                            \
   virtual bool isDerivedFrom(int baseid) const;                           \
   static NetObject* createObject();                                       \
   static RuntimeInfo* info;

/// Add this macro to the class implementation file which should be able to replicate
#define IMPLEMENT_REPLICATABLE(id, className, baseName)                    \
   RuntimeInfo* className::info = NetObjectFactory::getInstance().registerObject(id,className());\
   NetObject* className::createObject() { return new className(); }        \
   RuntimeInfo* className::getRuntimeInfo() const {                        \
      return className::info;                                              \
   }                                                                       \
   void className::initInfo(RuntimeInfo* info) const {                     \
      info->setBase(#baseName);                                            \
      info->setName(#className);                                           \
      info->setCreateObjectFnc(createObject);                              \
   }                                                                       \
   bool className::isDerivedFrom(int baseid) const {                       \
      return info->isDerivedFrom(baseid);                                  \
   }

#endif
