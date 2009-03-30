/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#include "profiler.h"

#include "..\..\defines.h"
#include "..\..\system\timer.h"

#include "..\..\gui\guitext.h"

#include "profileritem.h"

// singleton instance
Profiler Profiler::mInstance;
  
// static 
Profiler& Profiler::getInstance()
{
   return mInstance;
}

Profiler::Profiler():
   mItems(),
   mCallstack(),
   mpTimer(NULL),
   mDamping(0.8)
{
   mpTimer = new Timer();
}

Profiler::~Profiler()
{
   if ( mpTimer != NULL )
   {
      delete mpTimer;
      mpTimer = NULL;
   }
}

bool Profiler::hasParent() const
{
   return !mCallstack.empty();
}

ProfilerItem& Profiler::getParent()
{
   ASSERT(hasParent())
   return *mCallstack.top();
}

int Profiler::getParentCount() const
{
   return mCallstack.size();
}

bool Profiler::isCurrent(const ProfilerItem& item) const
{
   return mCallstack.top() == &item;
}

bool Profiler::contains(const std::string& name) const
{
   return mItems.find(name) != mItems.end();
}

void Profiler::push(ProfilerItem& item)
{
   mCallstack.push(&item);
}

void Profiler::pop()
{
   mCallstack.pop();
}

void Profiler::begin()
{
   mpTimer->begin();
}

void Profiler::end()
{
   double elapsedtime = mpTimer->getInterval();
   if ( elapsedtime == 0.0 )
      elapsedtime = 0.001;

   ItemMap::iterator it = mItems.begin();
   for ( ; it != mItems.end(); it++ )
   {
      ProfilerItem* pitem = it->second;
      pitem->updateHistory(mDamping, elapsedtime);
   }
}

// drawing

void Profiler::draw(GuiFont& font)
{
   char line[512];
   int y = 25;

   ItemMap::iterator it = mItems.begin();
   for ( ; it != mItems.end(); it++ )
   {
      ProfilerItem* pitem = it->second;
      pitem->asString(line);

      GuiText::printf(font, 25, y, line);
      y += 12;
   }
}

// Searching

ProfilerItem* Profiler::findOrCreateItem(const std::string& name)
{
   if ( !contains(name) )
   {
      mItems[name] = new ProfilerItem(name);
   }

   return mItems[name];
}

