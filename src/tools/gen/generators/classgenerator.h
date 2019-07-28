
#ifndef CLASS_GENERATOR_H
#define CLASS_GENERATOR_H

#include "generator.h"

class ClassGenerator : public Generator
{
public:
   virtual bool generate(CommandLine& commandline) override;

private:
 // helpers
   bool writeFile(const String& filename, const String& output);
};

#endif // CLASS_GENERATOR_H
