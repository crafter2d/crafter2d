
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

Compiler::Compiler():
   mContext(),
   mpCallback(NULL),
   mSteps()
{
   createSteps();
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
   String s = String(classname.c_str()).toLower();
   std::string lowercase = s.toStdString();

   if ( lowercase == "classloader" )
   {
      int aap = 0;
   }

   bool loaded = mContext.hasClass(lowercase);
   if ( !loaded )
   {
      AntlrParser parser(mContext);

      std::string filename = "..\\ascripts\\" + classname + ".as";
      mContext.getLog().info("> " + filename);

      AutoPtr<AntlrStream> stream(AntlrStream::fromFile(filename));
      AutoPtr<ASTRoot> root(parser.parse(*stream));
      if ( root.hasPointer() )
      {
         loaded = performSteps(*root);

         if ( loaded && hasCallback() )
         {
            mpCallback->notify(mContext.getResult());
         }
         else if ( !loaded)
         {
            const CompileLog::StringList& log = mContext.getLog().getLog();
            for ( std::size_t index = 0; index < log.size(); index++ )
            {
               std::cout << log[index] << std::endl;
            }
         }
         else
         {
            // save the file
         }
      }
      else
      {
         loaded = false;
      }
   }

   return loaded;
}

// - Operations
   
void Compiler::createSteps()
{
   mSteps.push_back(new PreloaderVisitor(*this));
   mSteps.push_back(new SymbolCollectorVisitor(mContext));
   mSteps.push_back(new SymbolCheckVisitor(mContext));
   mSteps.push_back(new ResourceCheckVisitor(mContext));
   mSteps.push_back(new OOCheckVisitor(mContext));
   mSteps.push_back(new CodeGeneratorVisitor(mContext));
}

bool Compiler::performSteps(ASTRoot& root)
{
   for ( std::size_t index = 0; index < mSteps.size(); index++ )
   {
      CompileStep* pstep = mSteps[index];
      if ( !pstep->step(root) || mContext.getLog().hasErrors() )
      {
         return false;
      }
   }
   return true;
}
