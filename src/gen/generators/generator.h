
#ifndef GENERATOR_H
#define GENERATOR_H

class CommandLine;
class FileSystem;

class Generator
{
public:
   Generator();
   virtual ~Generator();

 // get/set
   const FileSystem& getFileSystem() const;
   void              setFileSystem(FileSystem& filesystem);

 // generation
   virtual bool generate(CommandLine& commandline) = 0;

private:
   FileSystem* mpFileSystem;
};

#endif // GENERATOR_H
