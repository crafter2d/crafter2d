
#include "projectgenerator.h"

#include <fstream>
#include <direct.h>
#include <io.h>
#include <ctemplate/template.h>

#include "core/commandline/commandline.h"
#include "core/smartptr/autoptr.h"
#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"

using namespace std;

ProjectGenerator::ProjectGenerator():
   Generator()
{
}

// - Generation

bool ProjectGenerator::generate(CommandLine& commandline)
{
   const CommandLineArgument* ppackageargument = commandline.findArgument(String("package"));
   const CommandLineArgument& nameargument = commandline.getArgument(String("name"));
   bool force = commandline.hasArgument(String("force"));

   std::string projectname = nameargument.getValue().toStdString();
   std::string packagename = ppackageargument != NULL ? ppackageargument->getValue().toStdString() : projectname;

   ctemplate::TemplateDictionary dict("values");
   dict.SetValue("PROJ", projectname);
   dict.SetValue("PACKAGE", packagename);

   std::string clientoutput;
   std::string serveroutput;
   ctemplate::ExpandTemplate("../projects/templates/client.tpl", ctemplate::DO_NOT_STRIP, &dict, &clientoutput);
   ctemplate::ExpandTemplate("../projects/templates/server.tpl", ctemplate::DO_NOT_STRIP, &dict, &serveroutput);

   std::string dir = "../scripts/" + packagename;
   if ( access(dir.c_str(), 0) == -1 ) // dir does not exist yet
   {
      if ( mkdir(dir.c_str()) == -1 )
      {
         // can not create the directory
         return false;
      }
   }
   
   std::string clientfile = File::concat(packagename, projectname + "Client.as");
   std::string serverfile = File::concat(packagename, projectname + "Server.as");

   return write(clientfile, clientoutput, force) && write(serverfile, serveroutput, force);
}

// - Operations

bool ProjectGenerator::write(const std::string& filename, const std::string& output, bool force)
{
   if ( !force && getFileSystem().exists(filename) )
   {
      return false;
   }
   
   AutoPtr<File> file = getFileSystem().open(filename, File::EWrite | File::EText);
   file->write(output);
   
   return true;
}