
#include "compiler.h"

#include <iostream>

#include "core/string/string.h"
#include "core/conv/lexical.h"
#include "core/smartptr/autoptr.h"

#include "script/antlr/antlrinterface.h"
#include "script/antlr/antlrstream.h"

#include "steps/preloadervisitor.h"
#include "steps/symbolcollectorstep.h"
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

Compiler::Compiler():
   mContext(*this),
   mpCallback(NULL),
   mSteps(),
   mPhase(eLoad)
{
   createLoadSteps();
   createCompileSteps();
}

// - Get/set

bool Compiler::hasCallback() const
{
   return mpCallback != NULL;
}

CompileCallback& Compiler::getCallback()
{
   return *mpCallback;
}

void Compiler::setCallback(CompileCallback& callback)
{
   mpCallback = &callback;
}

// - Query

const Literal& Compiler::lookupLiteral(int index) const
{
   return mContext.getLiteralTable()[index];
}

// - Compilation

bool Compiler::compile(const std::string& classname)
{
   mContext.resetCollection();

   bool loaded = mContext.hasClass(classname) || loadClass(classname);
   if ( !loaded )
   {
      displayErrors(classname);
      return false;
   }

   for ( int index = 0; index < mFiles.size(); index++ )
   {
      if ( mFiles[index] == classname )
      {
         return true;
      }
   }

   ASTClass* pclass = mContext.findClass(classname);
   if ( performSteps(*pclass, mSteps) )
   {
      if ( hasCallback() )
         mpCallback->notify(mContext.getResult());

      mFiles.push_back(classname);

      save(*pclass);

      return true;
   }
   
   return false;
}

// - Operations

void Compiler::createLoadSteps()
{
   mLoadSteps.push_back(new SymbolCollectorVisitor(mContext));
}

void Compiler::createCompileSteps()
{
   mSteps.push_back(new SymbolCheckVisitor(mContext));
   mSteps.push_back(new ResourceCheckVisitor(mContext));
   mSteps.push_back(new OOCheckVisitor(mContext));
   mSteps.push_back(new CodeGeneratorVisitor(mContext));
}

bool Compiler::loadClass(const std::string& classname)
{
   AntlrParser parser(mContext);

   std::string fullname = mContext.getFullName(classname);
   if ( fullname.empty() )
      return false;

   String name(fullname.c_str());
   name.replace('.', '/');

   std::string filename = "ascripts/" + name.toStdString() + ".as";
   mContext.getLog().info("> " + filename);

   AutoPtr<AntlrStream> stream(AntlrStream::fromFile(filename));
   if ( stream.hasPointer() )
   {
      AutoPtr<ASTRoot> root(parser.parse(*stream));
      if ( root.hasPointer() )
      {
         return performSteps(*root, mLoadSteps);
      }
   }

   return false;
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

void Compiler::save(ASTClass& ast)
{
   // do some interesting saving stuff here
}

void Compiler::displayErrors(const std::string& currentfile)
{
   std::cout << "Error while compiling " << currentfile << std::endl;
   const CompileLog::StringList& log = mContext.getLog().getLog();
   for ( std::size_t index = 0; index < log.size(); index++ )
   {
      std::cout << log[index] << std::endl;
   }
}
