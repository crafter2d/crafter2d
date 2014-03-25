
#ifndef GENERATOR_H
#define GENERATOR_H

class String;
class CommandLine;

class Generator
{
public:
   Generator();
   virtual ~Generator();

 // generation
   virtual bool generate(CommandLine& commandline) = 0;

protected:

 // query
   String getWorkingDir() const;
   String getTemplateFile(const String& tpl) const;

};

#endif // GENERATOR_H
