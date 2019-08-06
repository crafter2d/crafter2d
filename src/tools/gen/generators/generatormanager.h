#ifndef GENERATOR_MANAGER_H
#define GENERATOR_MANAGER_H

#include <vector>

#include "generator.h"

namespace c2d::gen
{
   class GeneratorManager
   {
   public:
      static GeneratorManager& getInstance();

    // registrator
      void registerGenerator(Generator& generator);

    // lookup
      Generator* find(const String& command);

   private:

      std::vector<std::reference_wrapper<Generator>> mGenerators;
   };
}

#endif // GENERATOR_MANAGER_H
