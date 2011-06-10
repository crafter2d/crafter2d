
#include "compiler.h"

#include <iostream>
#include <fstream>

#include "core/string/string.h"
#include "core/conv/lexical.h"
#include "core/smartptr/autoptr.h"

#include "script/antlr/antlrinterface.h"
#include "script/antlr/antlrstream.h"

#include "exceptions/classnotfoundexception.h"

#include "steps/preloadvisitor.h"
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

// - Compilation

bool compare(ASTClass* pleft, ASTClass* pright)
{
   return pright->isBase(*pleft) || pright->isImplementing(*pleft);
}

void sort(std::vector<ASTClass*>& classes, std::vector<ASTClass*>& sorted)
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

bool Compiler::compile(const std::string& classname)
{
   ASTClass* pclass = mContext.findClass(classname);
   if ( pclass == NULL )
   {
      try
      {
         if ( !load(classname) )
         {
            displayErrors(classname);
            return false;
         }

         std::vector<ASTClass*> classes, sorted;
         mContext.collectCompileClasses(classes);
         ASSERT(!classes.empty());

         // precompile
         for ( std::size_t index = 0; index < classes.size(); index++ )
         {
            pclass = classes[index];
            if ( !performSteps(*pclass, mPrecompileSteps) )
            {
               displayErrors(classname);
               return false;
            }
         }

         // needs to have all types resolved
         sort(classes, sorted);

         // calculate the resources
         for ( std::size_t index = 0; index < sorted.size(); index++ )
         {
            pclass = sorted[index];
            pclass->calculateResources();
         }

         // compile
         for ( std::size_t index = 0; index < sorted.size(); index++ )
         {
            pclass = sorted[index];
            if ( performSteps(*pclass, mCompileSteps) )
            {
               if ( hasCallback() )
               {
                  mpCallback->notify(mContext.getResult());
               }

               save(*pclass);
            }
            else
            {
               displayErrors(pclass->getFullName());
               return false;
            }
         }
      }
      catch ( ClassNotFoundException* e )
      {
         mContext.getLog().error("Can not find class " + e->mClass);
         displayErrors(classname);
         return false;
      }
      catch ( std::exception& e )
      {
         mContext.getLog().error(e.what());
         displayErrors(classname);
         return false;
      }
   }

   return true;
}

// - Operations

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

bool Compiler::load(const std::string& classname)
{
   AntlrParser parser(mContext);

   String name(classname.c_str());
   name.replace('.', '/');

   std::string filename = "ascripts/" + name.toStdString() + ".as";
   mContext.getLog().info("> " + filename);

   try
   {
      AutoPtr<AntlrStream> stream(AntlrStream::fromFile(filename));
      if ( stream.hasPointer() )
      {
         AutoPtr<ASTRoot> root(parser.parse(*stream));
         if ( root.hasPointer() )
         {
            return performSteps(*root, mLoadSteps);
         }
      }
   }
   catch ( ClassNotFoundException* e )
   {
      mContext.getLog().error("Can not find class " + e->mClass);
      throw;
   }
   catch ( std::exception& e )
   {
      mContext.getLog().error(std::string("Exception: ") + e.what());
   }

   return false;
}

void Compiler::save(ASTClass& ast)
{
   // do some interesting saving stuff here
}

void Compiler::displayErrors(const std::string& currentfile)
{
   std::ofstream outfile("compilelog.txt", std::ios_base::app);

   outfile << "Error while compiling " << currentfile << std::endl;
   const CompileLog::StringList& log = mContext.getLog().getLog();
   for ( std::size_t index = 0; index < log.size(); index++ )
   {
      outfile << log[index] << std::endl;
   }

   outfile.close();
}
