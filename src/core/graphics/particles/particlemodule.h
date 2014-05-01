
#ifndef PARTICLE_MODULE_H
#define PARTICLE_MODULE_H

namespace Graphics
{
   class ParticleSystem;

   enum ModuleKind
   {
      eSpawnModule,
   };

   class ParticleModule
   {
   public:
      ParticleModule(ModuleKind kind);

    // get/set
      ModuleKind getKind() const;

   private:

    // data
      ModuleKind mKind;
   };
}

#endif // PARTICLE_MODULE_H
