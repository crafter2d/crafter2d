
#ifndef PARTICLES_H
#define PARTICLES_H

#include "core/defines.h"

#include "particle.h"

namespace Graphics
{
   class Particles
   {
   public:
      Particles():
         mppParticles(NULL), 
         mSize(0) {
      }

      ~Particles () {
         try
         {
            delete[] mppParticles;
         }
         catch ( ... ) {}
      }

    // query
      const Particle& operator[](int index) const {
         return *mppParticles[index];
      }

      Particle& operator[](int index) {
         return *mppParticles[index];
      }

      int size() const {
         return mSize;
      }

    // operations
      void initialize(int size) {
         mSize = size;
         mppParticles = new Particle * [size];
         for ( int index = 0; index < size; ++index ) {
            mppParticles[index] = new Particle();
         }
      }

      void swap(int left, int right) {
         auto ptemp = mppParticles[left];
         mppParticles[left] = mppParticles[right];
         mppParticles[right] = ptemp;
      }
      

   private:

    // data
      Particle** mppParticles;
      int        mSize;
   };
}

#endif // PARTICLES_H
