
#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include "generator.h"

class String;

class ProjectGenerator : public Generator
{
public:
   ProjectGenerator();

 // generation
   virtual bool generate(CommandLine& commandline);

private:
 // operations
   bool write(const String& filename, const String& output, bool force);
};

#endif // PROJECT_GENERATOR_H
