/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "commandlineargument.h"

CommandLineArgument::CommandLineArgument():
   mType(eCommand),
   mName(),
   mValue()
{
}

CommandLineArgument::CommandLineArgument(const CommandLineArgument& that):
   mType(that.mType),
   mName(that.mName),
   mValue(that.mValue)
{
}

// - Get/set

CommandLineArgument::Type CommandLineArgument::getType() const
{
   return mType;
}

void CommandLineArgument::setType(Type type)
{
   mType = type;
}

const String& CommandLineArgument::getName() const
{
   return mName;
}

void CommandLineArgument::setName(const String& name)
{
   mName = name;
}

const String& CommandLineArgument::getValue() const
{
   return mValue;
}

void CommandLineArgument::setValue(const String& value)
{
   mType = eArgument;
   mValue = value;
}
