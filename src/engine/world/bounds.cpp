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
#include "bounds.h"

#include "core/defines.h"

#include "bound.h"

Bounds::Bounds():
   BoundsImp()
{
}

Bounds::~Bounds()
{
}

//////////////////////////////////////////////////////////////////////////
// - (De)registration
//////////////////////////////////////////////////////////////////////////

void Bounds::closeGaps(float delta)
{
   Bound* pother = NULL;

   for ( int idx = 0; idx < size(); ++idx )
   {
      Bound* pbound = operator[](idx);

      Side side = findNeighbour(pbound, delta, pother);

      if ( side == eLeft )
      {
         pother->setLeft(pbound->getRight());
      }
      else if ( side == eRight )
      {
         pother->setRight(pbound->getLeft());
      }
   }
}

Bounds::Side Bounds::findNeighbour(Bound* porig, float delta, Bound*& pneighbour)
{
   for ( int index = 0; index < size(); ++index )
   {
      Bound* pbound = operator[](index);

      if ( pbound != porig )
      {
         if ( pbound->getLeft().distance(porig->getRight()) < delta )
         {
            pneighbour = pbound;

            return eLeft;
         }
         if ( pbound->getRight().distance(porig->getLeft()) < delta )
         {
            pneighbour = pbound;
            return eRight;
         }
      }
   }

   return eInvalid;
}
