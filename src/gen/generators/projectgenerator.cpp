
#include "projectgenerator.h"

#include <fstream>
#include <direct.h>
#include <io.h>
#include <iostream>
#include <ctemplate/template.h>

#include "core/commandline/commandline.h"
#include "core/smartptr/autoptr.h"
#include "core/string/string.h"
#include "core/vfs/stdiofile.h"
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

   std::string projectname = nameargument.getValue().toUtf8();
   std::string packagename = ppackageargument != NULL ? ppackageargument->getValue().toUtf8() : projectname;

   ctemplate::TemplateDictionary dict("values");
   dict.SetValue("PROJ", projectname);
   dict.SetValue("PACKAGE", packagename);

   std::string clienttpl = getTemplateFile(UTEXT("client")).toUtf8();
   std::string servertpl = getTemplateFile(UTEXT("server")).toUtf8();

   std::string clientoutput;
   std::string serveroutput;
   if ( !ctemplate::ExpandTemplate(clienttpl, ctemplate::DO_NOT_STRIP, &dict, &clientoutput)
     || !ctemplate::ExpandTemplate(servertpl, ctemplate::DO_NOT_STRIP, &dict, &serveroutput) )
   {
      std::cerr << "Could not load or expand the script template." << std::endl;
      return false;
   }

   // The path that we assume here is: projectdir\scripts\package
   // where projectdir is the directory of a Crafter Workshop project
   
   const CommandLineArgument& pathargument = commandline.getArgument(UTEXT("path"));
   String path = File::concat(File::concat(pathargument.getValue(), UTEXT("scripts")), String(packagename));
   int result = FileSystem::getInstance().mkdir(path);
   if ( result == ERR_PATH_NOT_FOUND )
   {
      // path was incorrect
      std::cerr << "Could not create the package folder." << std::endl;
      return false;
   }

   // now create the client & server files: ProjectNameClient.as and ProjectNameServer.as
   String clientfile = File::concat(path, String(projectname) + UTEXT("Client.as"));
   String serverfile = File::concat(path, String(projectname) + UTEXT("Server.as"));

   String servercontent = String::fromUtf8(serveroutput.c_str());
   String clientcontent = String::fromUtf8(clientoutput.c_str());

   return write(clientfile, clientcontent, force) 
       && write(serverfile, servercontent, force);
}

// - Operations

bool ProjectGenerator::write(const String& filename, const String& output, bool force)
{
   if ( !force && FileSystem::getInstance().exists(filename) )
   {
      return false;
   }
   
   StdioFile file;
   if ( file.open(filename, File::EWrite | File::EText) )
   {
      file.write(output);
      return true;
   }

   std::cerr << "Could not write to file " << filename.toUtf8() << std::endl;
   
   return false;
}
