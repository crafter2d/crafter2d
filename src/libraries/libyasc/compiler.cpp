
#include "compiler.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <libcil/cil/class.h>
#include <libcil/cil/codestreamer.h>
#include <libcil/common/type.h>

#include "core/string/string.h"
#include "core/string/stringlist.h"
#include "core/string/stringinterface.h"
#include "core/conv/lexical.h"
#include "core/streams/filewriterstream.h"
#include "core/system/platform.h"
#include "core/vfs/stdiofile.h"
#include "core/vfs/filesystem.h"
#include "core/defines.h"

#include "antlr/antlrexception.h"
#include "antlr/antlrinterface.h"

#include "exceptions/classnotfoundexception.h"
#include "exceptions/filenotfoundexception.h"

#include "steps/preloadvisitor.h"
#include "steps/symbolcheckstep.h"
#include "steps/resourcecheckvisitor.h"
#include "steps/oocheckvisitor.h"
#include "steps/codegeneratorvisitor.h"

#include "compilecallback.h"

/// Compilation will be split up in two phases in order to support multiple files to point to each other.
/// During this change the usage of the use keyword will also change. These will be stored in a table and
/// be used as a full path for the class. The first 'use' that matches, will be used to load the class.
///
/// 1e Phase: load AST of all required classes -> SymbolCollectorStep becomes phase 1
/// 2e Phase: compile each class that is loaded -> Other steps are part of phase 2
///
/// So, only after phase 1 is complete the other steps are performed for each class. By using the collector
/// step the preloader is not required anymore.

Compiler::Compiler(int argc, char* pargv[]):
   mCommands(),
   mContext(*this),
   mFileLookup(),
   mCompileSteps(),
   mPhase(ePreload),
   mOutputDir()
{
   mCommands.set(argc, pargv);

   createCompileSteps();
}

Compiler::~Compiler()
{
   cleanUp();
}

// - Operations

void Compiler::cleanUp()
{
   for ( std::size_t index = 0; index < mCompileSteps.size(); index++ )
   {
      delete mCompileSteps[index];
   }
}

// - Compilation

int Compiler::exec()
{
   std::cout << "Yet Another Compiler, Copyright J.Broekhuizen, 2013" << std::endl;

   if ( mCommands.size() == 0 )
   {
      std::cout << "No arguments specified" << std::endl;
      return 2;
   }

   if ( mCommands.hasArgument(UTEXT("help")) )
   {
      printHelp();
      return 0;
   }

   FileSystem::getInstance().addPath(UTEXT("."));

   // last argument must be the file mask to compile
   String mask = File::toNativeSeparator(mCommands[mCommands.size() - 1].getName());
   int pos = mask.lastIndexOf(FileSystem::getNativeSeparator());
   if ( pos > 0 )
   {
      String path = mask.subStr(0, pos);
      FileSystem::getInstance().addPath(path);
   }

   // get the additional paths 
   const CommandLineArgument* prootarg = mCommands.getArgument(UTEXT("p"));
   if ( prootarg != nullptr )
   {
      String roots = prootarg->getValue();
      auto rootpaths = StringInterface::tokenize(roots, L';');

      auto fnc = [this](const String& path) { FileSystem::getInstance().addPath(path); };
      std::for_each(rootpaths.begin(), rootpaths.end(), fnc);
   }

   // see if we should find the files recursively
   bool recursive = false;
   const CommandLineArgument* precursearg = mCommands.getArgument(UTEXT("r"));
   if ( precursearg != nullptr )
   {
      recursive = true;
   }

   // find the output dir
   const CommandLineArgument* parg = mCommands.getArgument(UTEXT("o"));
   if ( parg != nullptr )
   {
      mOutputDir = parg->getValue();
      FileSystem::getInstance().mkpath(mOutputDir);
   }
   else
   {
      mOutputDir = UTEXT(".");
   }

   // preload required internal classes

   mContext.loadClass(UTEXT("system.InternalArray"));
   mContext.loadClass(UTEXT("system.InternalString"));

   // compile all files based on the mask

   std::vector<String> files;
   FileSystem::getInstance().find(mask, files, recursive);

   bool result = true;
   auto compilefnc = [this, &result](const String& name) { if ( !compile(name) ) { result = false; } };
   std::for_each(files.begin(), files.end(), compilefnc);

   return result ? 0 : 1;
}

bool Compiler::compile(const String& filename)
{
   bool success = false;
   try
   {
      StdioFile file;
      if ( !file.open(filename, File::ERead | File::EText) )
      {
         return false;
      }

      ASTRoot& root = load(file);
      if ( performSteps(root, mCompileSteps) )
      {
         std::vector<ASTClass*> classes;
         root.collectClasses(classes);

         for ( auto* pclass : classes )
         {
            save(*pclass);
         }

         success = true;
      }
      
      reportLogEntries();
   }
   catch ( CompileException* pexception )
   {
      reportException(*pexception);
   }
   catch ( std::exception* )
   {
      // log it!
   }

   return success;
}

// - Operations

void Compiler::createCompileSteps()
{
   mCompileSteps.push_back(new SymbolCheckVisitor(mContext));
   mCompileSteps.push_back(new ResourceCheckVisitor(mContext));
   mCompileSteps.push_back(new OOCheckVisitor(mContext));
   mCompileSteps.push_back(new CodeGeneratorVisitor(mContext));
}

bool Compiler::performSteps(ASTNode& node, Steps& steps)
{
   for ( std::size_t index = 0; index < steps.size(); index++ )
   {
      CompileStep* pstep = steps[index];
      if ( !pstep->step(node) || mContext.getLog().hasErrors() )
      {
         return false;
      }
   }

   return true;
}

ASTRoot& Compiler::load(const String& classname)
{
   String filename(classname);
   filename.replace('.', FileSystem::getNativeSeparator());
   filename += UTEXT(".as");

   std::unique_ptr<File> file(FileSystem::getInstance().open(filename, File::ERead | File::EText));
   if ( !file )
   {
      throw new FileNotFoundException(filename);
   }

   return load(*file);
}

ASTRoot& Compiler::load(File& file)
{
   try
   {
      FileLookup::iterator it = mFileLookup.find(file.getFileName());
      if ( it != mFileLookup.end() )
      {
         return *it->second;
      }
      else
      {
         AntlrParser parser(mContext);
         ASTRoot* proot = parser.parse(file);
         mFileLookup.insert(std::make_pair(file.getFileName(), proot));
         PreloadVisitor visitor(mContext);
         proot->accept(visitor);
         return *proot;
      }
   }
   catch ( CompileException* pexception )
   {
      pexception->setFilename(file.getFileName());
      reportException(*pexception);
   }

   throw new FileNotFoundException(file.getFileName());
}

void Compiler::save(ASTClass& ast)
{
   std::unique_ptr<CIL::Class> klass(ast.useCompiledClass());
   if ( klass )
   {
      String filename = File::concat(mOutputDir, ast.getFullName()) + UTEXT(".cas");
      StdioFile file;
      if ( file.open(filename, File::EWrite | File::EBinary) )
      {
         FileWriterStream filestream(file);
         CIL::CodeStreamer streamer(filestream);
         streamer << *klass;
      }
   }
}

// - Error logging

void Compiler::reportException(CompileException& exception)
{
   std::ofstream outfile("compilelog.txt", std::ios_base::app);

   outfile << "> " << exception.getFilename().toUtf8() << "(" << exception.getLine() << "): " << exception.asString().toUtf8() << std::endl;
}

void Compiler::reportLogEntries()
{
   //std::ofstream outfile("compilelog.txt", std::ios_base::app);

   const StringList& log = mContext.getLog().getLog();
   if ( !log.isEmpty() )
   {
      //auto fnc = [this,&outfile](const String& msg) { outfile << msg.toUtf8() << std::endl; };
      auto fnc = [this](const String& msg) { std::cout << msg.toUtf8() << std::endl; };
      std::for_each(log.begin(), log.end(), fnc);

      mContext.getLog().clear();
   }
}

// - Argument handling

void Compiler::printHelp()
{
   std::cout << "Yasc [options] filemask" << std::endl;
   std::cout << "  filemask\tOptional path followed by filename or file mask (*). If a path is present it is used as source path." << std::endl << std::endl;
   std::cout << "Options" << std::endl;
   std::cout << "  -r\tRecursive lookup for files" << std::endl;
   std::cout << "  -p\tAdd additional source paths (seperated by semi-colon)" << std::endl;
   std::cout << "  -o\tOutput directory of the compiled sources" << std::endl;
}
