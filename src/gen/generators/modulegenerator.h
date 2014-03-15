
#ifndef MODULE_GENERATOR_H
#define MODULE_GENERATOR_H

#include "generator.h"

class ModuleGenerator : public Generator
{
public:
   virtual bool generate(CommandLine& commandline) override;
};

#endif // MODULE_GENERATOR_H
