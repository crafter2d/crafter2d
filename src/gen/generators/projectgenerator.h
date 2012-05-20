
#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include "generator.h"

#include <string>

class ProjectGenerator : public Generator
{
public:
   ProjectGenerator();

 // generation
   virtual bool generate(CommandLine& commandline);

private:
 // operations
   bool write(const std::string& filename, const std::string& output, bool force);
};

#endif // PROJECT_GENERATOR_H
