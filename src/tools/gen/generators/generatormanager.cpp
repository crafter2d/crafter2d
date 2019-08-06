
#include "generatormanager.h"

#include "core/string/string.h"

namespace c2d::gen
{
   // static
   GeneratorManager& GeneratorManager::getInstance()
   {
      static GeneratorManager mInstance;
      return mInstance;
   }

   void GeneratorManager::registerGenerator(Generator& generator)
   {
      mGenerators.push_back(std::ref(generator));
   }

   Generator* GeneratorManager::find(const String& command)
   {
      if ( command.length() == 1 )
      {
         UChar cmd = command[0];
         for ( auto& genref : mGenerators )
         {
            if ( genref.get().getBriefCommand() == cmd )
            {
               return &genref.get();
            }
         }
      }
      else
      {
         for ( auto& genref : mGenerators )
         {
            auto& generator = genref.get();
            if ( generator.getCommand() == command )
            {
               return &generator;
            }
         }
      }
      return nullptr;
   }
}
