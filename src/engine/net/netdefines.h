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
   ConnectEventId          = 3,
   ConnectReplyEventId     = 4,
   JoinEventId             = 5,
   DisconnectEventId       = 6,
   ActionEventId           = 7,
   ViewportEventId         = 8,
   NewObjectEventId        = 9,
   DeleteObjectEventId     = 10,
   RequestObjectEventId    = 11,
   UpdateObjectEventId     = 12,
   ScriptEventId           = 13,
   NameChangeObjectEventId = 14,
   ServerDownEventId       = 15,
   WorldChangedEventId     = 16,
   AggregateEventId        = 17,

   EntityId                = 101,
   ActorId                 = 102,
   AnimObjectId            = 103,
   CreatureId              = 104,
   WorldId                 = 105,
   ParticleSystemId        = 106,
   
   LaserId                 = 501
};

/// Add this macro to the class definition which should be able to replicate
#define DEFINE_REPLICATABLE(className)                                     \
   virtual RuntimeInfo& getRuntimeInfo() const;                            \
   virtual bool isDerivedFrom(int baseid) const;                           \
   static RuntimeInfo info;

/// Add this macro to the class implementation file which should be able to replicate
#define ABSTRACT_IMPLEMENT_REPLICATABLE(id, className, baseName)           \
   RuntimeInfo className::info(id, #className, #baseName, NULL);           \
   RuntimeInfo& className::getRuntimeInfo() const {                        \
      return info;                                                         \
   }                                                                       \
   bool className::isDerivedFrom(int baseid) const {                       \
      return info.isDerivedFrom(baseid);                                   \
   }

/// Add this macro to the class implementation file which should be able to replicate
#define IMPLEMENT_REPLICATABLE(id, className, baseName)                                \
   static NetObject* className##_createObject() { return new className(); }            \
   RuntimeInfo className::info(id, #className, #baseName, className##_createObject);   \
   RuntimeInfo& className::getRuntimeInfo() const {                                    \
      return info;                                                                     \
   }                                                                                   \
   bool className::isDerivedFrom(int baseid) const {                                   \
      return info.isDerivedFrom(baseid);                                               \
   }

#endif
