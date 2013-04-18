
#include "compiler.h"

#include <iostream>
#include <fstream>

#include "core/string/string.h"
#include "core/conv/lexical.h"
#include "core/smartptr/autoptr.h"

#include "script/antlr/antlrexception.h"
#include "script/antlr/antlrinterface.h"
#include "script/antlr/antlrstream.h"

#include "exceptions/classnotfoundexception.h"
#include "steps/preloadvisitor.h"
#include "steps/symbolcollectorstep.h"
#include "steps/symbolcheckstep.h"
#include "steps/resourcecheckvisitor.h"
#include "steps/oocheckvisitor.h"
#include "steps/codegeneratorvisitor.h"
#include "steps/bytecodegenerationvisitor.h"

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
   mLoadSteps(),
   mPrecompileSteps(),
   mCompileSteps(),
   mPhase(ePreload)
{
   createLoadSteps();
   createPrecompileSteps();
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

// - Operations

void Compiler::cleanUp()
{
   for ( std::size_t index = 0; index < mLoadSteps.size(); index++ )
   {
      delete mLoadSteps[index];
   }

   for ( std::size_t index = 0; index < mPrecompileSteps.size(); index++ )
   {
      delete mPrecompileSteps[index];
   }

   for ( std::size_t index = 0; index < mCompileSteps.size(); index++ )
   {
      delete mCompileSteps[index];
   }
}

// - Compilation

bool Compiler::compile(const String& classname)
{
   bool success = true;
   ASTClass* pclass = mContext.findClass(classname);
   if ( pclass == NULL )
   {
      try
      {
         // STEP 1: load the file(s)
         if ( !load(classname) )
         {
            return false;
         }

         ASTClasses classes, sorted;
         mContext.collectCompileClasses(classes);
         ASSERT_MSG(!classes.empty(), "There should at least be one class to compile!?");

         // STEP 2: precompile & sort the loaded files
         for ( std::size_t index = 0; index < classes.size(); index++ )
         {
            pclass = classes[index];
            performSteps(*pclass, mPrecompileSteps);
            if ( mContext.getLog().hasErrors() )
            {
               displayErrors(pclass->getFullName());
               return false;
            }
         }

         // needs to have all types resolved
         sort(classes, sorted);

         // STEP 3: calculate the resources
         for ( std::size_t index = 0; index < sorted.size(); index++ )
         {
            pclass = sorted[index];
            pclass->calculateResources();
         }

         // STEP 4: compile the classes
         for ( std::size_t index = 0; index < sorted.size(); index++ )
         {
            pclass = sorted[index];
            if ( performSteps(*pclass, mCompileSteps) )
            {
               save(*pclass);

               /*

               if ( hasCallback() )
               {
                  mpCallback->notify(pcilclass);
               }

               save(*pcilclass); */
            }
            else
            {
               ASSERT(mContext.getLog().hasErrors());
               displayErrors(pclass->getFullName());
               success = false;
            }
         }
      }
      catch ( CompileException* pexception )
      {
         reportError(*pexception);
         success = false;
      }
   }

   return success;
}

// - Operations

const ClassRegistry& Compiler::getClassRegistry() const
{
   return mContext.getClassRegistry();
}

void Compiler::setClassRegistry(const ClassRegistry& registry)
{
   mContext.setClassRegistry(registry);
}

void Compiler::createLoadSteps()
{
   mLoadSteps.push_back(new PreloadVisitor(mContext));
}

void Compiler::createPrecompileSteps()
{
   mPrecompileSteps.push_back(new SymbolCollectorVisitor(mContext));
}

void Compiler::createCompileSteps()
{
   mCompileSteps.push_back(new SymbolCheckVisitor(mContext));
   mCompileSteps.push_back(new ResourceCheckVisitor(mContext));
   mCompileSteps.push_back(new OOCheckVisitor(mContext));
   mCompileSteps.push_back(new CodeGeneratorVisitor(mContext));
   mCompileSteps.push_back(new ByteCodeGenerationVisitor(mContext));
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

bool Compiler::load(const String& classname)
{
   AntlrParser parser(mContext);

   String filename(classname);
   filename.replace('.', '/');
   filename += String(".as");

   try
   {
      AutoPtr<AntlrStream> stream(AntlrStream::fromFile(filename));
      if ( stream.hasPointer() )
      {
         AutoPtr<ASTRoot> root(parser.parse(*stream));
         if ( root.hasPointer() && performSteps(*root, mLoadSteps) )
         {
            root->detachClasses();
            return true;
         }
      }
   }
   catch ( CompileException* pexception )
   {
      pexception->setFilename(filename);
      reportError(*pexception);
   }

   return false;
}

void Compiler::save(const ASTClass& ast)
{
   // do some interesting saving stuff here
}

static bool compare(ASTClass* pleft, ASTClass* pright)
{
   return pright->isBase(*pleft) || pright->isImplementing(*pleft);
}

void Compiler::sort(ASTClasses& classes, ASTClasses& sorted)
{
   for ( std::size_t index = 0; index < classes.size(); index++ )
   {
      ASTClass* pleft = classes[index];

      bool inserted = false;
      std::vector<ASTClass*>::iterator it = sorted.begin();
      for ( ; it != sorted.end(); it++ )
      {
         ASTClass* pright = (*it);

         if ( compare(pleft, pright) )
         {
            // pleft = base class
            sorted.insert(it, pleft);
            inserted = true;
            break;
         }
      }

      if ( !inserted )
      {
         sorted.push_back(pleft);
      }
   }
}

void Compiler::reportError(CompileException& exception)
{
   std::ofstream outfile("compilelog.txt", std::ios_base::app);

   outfile << "> " << exception.getFilename().toStdString() << "(" << exception.getLine() << "): " << exception.asString().toStdString() << std::endl;
}

void Compiler::displayErrors(const String& currentfile)
{
   std::ofstream outfile("compilelog.txt", std::ios_base::app);

   const CompileLog::StringList& log = mContext.getLog().getLog();
   for ( std::size_t index = 0; index < log.size(); index++ )
   {
      outfile << "> " << currentfile.toStdString() << ": " << log[index] << std::endl;
   }

   outfile.close();

   mContext.getLog().clear();
}
