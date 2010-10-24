/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* This file contains the prototypes for the toLua program to bind these
 * classes to Lua.
 */
$#include "net/netconnection.h"
$#include "server.h"
$#include "client.h"
$#include "actionmap.h"
$#include "process.h"
$#include "scenegraph.h"
$#include "player.h"

#define INVALID_CLIENTID -1

class BitStream
{
   BitStream();
   ~BitStream();

   void clear();

   std::string readString();
   int readInt();
   bool readBool();

   void writeInt(int i);
   void writeString(const std::string& s);
   void writeBool(bool b);
};

class NetConnection
{
   bool create(int port=0);
   void disconnect();

   void send(BitStream* stream);
};

class Player
{
   Player();

   int client;
   Object* controler;
   std::string name;
};

class Process
{
   bool create();
   void destroy();

   NetConnection* getConnection();
   SceneGraph& getSceneGraph();
   ActionMap* getActionMap();

   void setActionMap(ActionMap* map);
   void sendScriptEvent(BitStream* stream, int client=-1);
   
   bool loadWorld(const std::string& filename, const std::string& name);
};

class Server: public Process
{
   void addPlayer(int client, Player* player);
   void sendScriptEventToAllClients(BitStream* stream);
};

class Client: public Process
{
   bool connect(const char* serverName, int port, const char* name);
   void disconnect();
   
   void setKeyMap(KeyMap* pkeymap);
};