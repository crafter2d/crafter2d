
#ifndef MODULE_GENERATOR_H
#define MODULE_GENERATOR_H

#include "generator.h"

class String;

class ModuleGenerator : public Generator
{
public:
   virtual bool generate(CommandLine& commandline) override;

private:

 // file generation
   bool generateContentModule(CommandLine& commandline, const String& path, const String& name);
};

#endif // MODULE_GENERATOR_H
