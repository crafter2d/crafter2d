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
#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include <vector>

namespace c2d
{
   class Sprite;

   class SpriteBatch
   {
   public:

      using Sprites = std::vector<const Sprite*>;
      using iterator = Sprites::iterator;

    // query
      int size() const {
         return mSprites.size();
      }

      const Sprite* front() {
         return mSprites.front();
      }

      iterator begin() {
         return mSprites.begin();
      }

      iterator end() {
         return mSprites.end();
      }

    // maintenance
      void add(const Sprite& sprite) {
         mSprites.push_back(&sprite);
      }

      void clear() {
         mSprites.clear();
      }

      void sort();

   private:
      
    // data
      Sprites mSprites;
   };
}

#endif // SPRITE_BATCH_H
