
#include "antlrinterface.h"

#include <iostream>
#include <memory>

#include "core/defines.h"
#include "core/conv/numberconverter.h"
#include "core/string/string.h"
#include "core/system/variant.h"

#include "yasc/compiler/output/yasLexer.h"
#include "yasc/compiler/output/yasParser.h"
#include "yasc/compiler/ast/astannotation.h"
#include "yasc/compiler/compilecontext.h"
#include "yasc/common/literal.h"

#include "antlrexception.h"
#include "antlrnode.h"
#include "antlrstream.h"

static void reportError(pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames);

AntlrParser::AntlrParser(CompileContext& context):
   mContext(context),
   mClassResolver(),
   mPackage(),
   mpClass(nullptr)
{
}

// - Parse

ASTRoot* AntlrParser::parse(File& file)
{
   std::unique_ptr<AntlrStream> stream(AntlrStream::fromFile(file));
   pyasLexer lexer = yasLexerNew(stream->getStream());
   if ( lexer == nullptr )
   {
      throw new AntlrException(UTEXT("failed to instantiate lexer"));
   }

   pANTLR3_COMMON_TOKEN_STREAM tstream;
   tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
   if ( tstream == nullptr )
   {
      lexer->free(lexer);

      throw new AntlrException(UTEXT("Can not open token stream"));
   }

   pyasParser parser = yasParserNew(tstream);
   if ( parser == nullptr )
   {
      lexer->free(lexer);
      tstream->free(tstream);

      throw new AntlrException(UTEXT("Failed to instantiate parser"));
   }

   try
   {
      parser->pParser->rec->displayRecognitionError = reportError;
      yasParser_script_return ast = parser->script(parser);

      if ( parser->pParser->rec->state->errorCount > 0 )
      {
         throw new AntlrException(UTEXT("Errors while parsing input"));
      }
      else
      {
         std::unique_ptr<ASTRoot> astroot(new ASTRoot());

         AntlrNode root(ast.tree);
         if ( !root.isNil() )
         {
            astroot->addChild(handleTree(root));
         }
         else
         {
            int count = root.getChildCount();
            for ( int index = 0; index < count; index++ )
            {
               AntlrNode declnode = root.getChild(index);
               astroot->addChild(handleTree(declnode));
            }
         }

         parser->free(parser);
         tstream->free(tstream);
         lexer->free(lexer);

         return astroot.release();
      }
   }
   catch ( AntlrException* pexception )
   {
      parser->free(parser);
      tstream->free(tstream);
      lexer->free(lexer);

      throw;
   }

   return nullptr;
}

ASTType* AntlrParser::getType(const AntlrNode& node)
{
   ASSERT(node.getType() == TYPE);

   AntlrNode typenode = node.getChild(0);

   ASTType* ptype = new ASTType();
   ptype->setPosition(typenode.getPosition());

   int nodetype = typenode.getType();
   switch ( nodetype )
   {
      case T_INT:
         ptype->setKind(ASTType::eInt);
         break;
      case T_REAL:
         ptype->setKind(ASTType::eReal);
         break;
      case T_CHAR:
         ptype->setKind(ASTType::eChar);
         break;
      case T_STRING:
         ptype->setKind(ASTType::eString);
         break;
      case T_BOOL:
         ptype->setKind(ASTType::eBoolean);
         break;
      case T_VOID:
         ptype->setKind(ASTType::eVoid);
         break;
      case ID:
         {
            String name = typenode.toString();
            String qualifiedname = mClassResolver.resolve(name);
            if ( qualifiedname.isEmpty() )
            {
               // can be a type argument!
               qualifiedname = name;
            }

            ptype->setKind(ASTType::eObject);
            ptype->setObjectName(qualifiedname);
         }
         break;
   }

   int count = node.getChildCount();
   if ( count > 1 )
   {
      AntlrNode restnode = node.getChild(1);

      nodetype = restnode.getType();
      if ( nodetype == TYPEARGUMENT )
      {
         int typearguments = restnode.getChildCount();
         for ( int index = 0; index < typearguments; index++ )
         {
            AntlrNode typeargument = restnode.getChild(index);

            ASTType* pargumenttype = getType(typeargument);

            ptype->addTypeArgument(pargumenttype);
         }
      }
      else // either generic or array, cant have array of generics
      {
         ASTType* parraytype = new ASTType();
         parraytype->setKind(ASTType::eArray);
         parraytype->setArrayType(ptype);
         parraytype->setArrayDimension((count - 1) / 2);

         ptype = parraytype;
      }
   }

   return ptype;
}

ASTNode* AntlrParser::handleTree(const AntlrNode& node)
{
   ASSERT(!node.isNil());

   int nodetype = node.getType();
   switch ( nodetype )
   {
      case T_PACKAGE:         return handlePackage(node);
      case USE:               return handleUse(node);
      case CLASS:             return handleClass(node);
      case INTRFACE:          return handleClass(node);
      case IFACE_MEMBER:      return handleInterfaceMember(node);
      case IFACE_VOID_MEMBER: return handleInterfaceVoidMember(node);
      case VARIABLE_DECL:     return handleVarDecl(node);
      case PROPERTY_DECL:     return handlePropertyDecl(node);
      case CONSTRUCTOR_DECL:  return handleConstructor(node);
      case FUNCTION_DECL:     return handleFuncDecl(node);
      case VOID_FUNCDECL:     return handleVoidFuncDecl(node);
      case ANNOTATIONS:       return handleAnnotations(node);
      case BLOCK:             return handleBlock(node);
      case LOCALVARDECL:      return handleLocalVarDecl(node);
      case ARRAYINIT:         return handleArrayInit(node);
      case IF:                return handleIf(node);
      case FOR:               return handleFor(node);
      case FOREACH:           return handleForeach(node);
      case WHILE:             return handleWhile(node);
      case DO:                return handleDo(node);
      case SWITCH:            return handleSwitch(node);
      case CASE:              return handleCase(node);
      case DEFAULT:           return handleCase(node);
      case RETURN:            return handleReturn(node);
      case TRY:               return handleTry(node);
      case THROW:             return handleThrow(node);
      case T_ASSERT:          return handleAssert(node);
      case STMT_EXPR:         return handleExpressionStatement(node);
      case COMPOUNT:          return handleCompound(node);
      case EXPRESSION:        return handleExpression(node);
      case INST_OF:           return handleInstanceOf(node);
      case UNARY:             return handleUnary(node);
      case NEW:               return handleNew(node);
      case SUPER:             return handleSuper(node);
      case THIS:              return handleSuper(node);
      case NATIVE:            return handleNative(node);
      case CAST:              return handleCast(node);
      case ACCESS:            return handleAccess(node);
      case ARRAYACCESS:       return handleArrayAccess(node);
      case CLASSACCESS:       return handleClassAccess(node);
      case BREAK:
      case CONTINUE:          return handleLoopControl(node);
      case EQUAL:
      case UNEQUAL:
      case SEQUAL:
      case SMALLER:
      case GREATER:
      case GEQUAL:
      case SHIFTL:
      case SHIFTR:
      case AND:
      case OR:
      case BITOR:
      case BITXOR:
      case BITAND:
      case MUL:
      case DIV:
      case REM:
      case PLUS:
      case MINUS:             return handleConcatenate(node);
      case LITERAL:           return handleLiteral(node);

      case TYPE:
      default:                UNREACHABLE("Should not get here");
         break;
   }

   return nullptr;
}

ASTNode* AntlrParser::handlePackage(const AntlrNode& node)
{
   String identifier;
   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode namenode = node.getChild(index);
      identifier += namenode.toString();

      if ( index < count - 1 )
      {
         identifier += '.';
      }
   }

   ASTPackage* ppackage = new ASTPackage();
   ppackage->setName(identifier);
   ppackage->setPosition(node.getPosition());

   mPackage = identifier;
   mClassResolver.insert(mPackage + UTEXT(".*"));

   return ppackage;
}

ASTNode* AntlrParser::handleUse(const AntlrNode& node)
{
   String identifier;
   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode namenode = node.getChild(index);
      identifier += namenode.toString();

      if ( index < count - 1 )
      {
         identifier += '.';
      }
   }

   ASTUse* puse = new ASTUse();
   puse->setIdentifier(identifier);
   puse->setPosition(node.getPosition());

   mClassResolver.insert(puse->getIdentifier());

   return puse;
}

void AntlrParser::handleModifiers(const AntlrNode& node, ASTModifiers& modifiers)
{
   ASSERT(node.getType() == MODIFIERS);

   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode child = node.getChild(index);
      int nodetype = child.getType();

      switch ( nodetype )
      {
         case PUBLIC:
            modifiers.setVisibility(ASTModifiers::ePublic);
            break;
         case PROT:
            modifiers.setVisibility(ASTModifiers::eProtected);
            break;
         case PRIVATE:
            modifiers.setVisibility(ASTModifiers::ePrivate);
            break;
         case STATIC:
            modifiers.setStatic();
            break;
         case FINAL:
            modifiers.setFinal();
            break;
         case ABSTRACT:
            modifiers.setAbstract();
            break;
         case NATIVE:
            modifiers.setNative();
            break;
      }
   }
}

ASTTypeVariables* AntlrParser::handleTypeVariables(const AntlrNode& node)
{
   ASTTypeVariables* ptypes = new ASTTypeVariables();

   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      String id, extra;
      ASTTypeVariable* ptype = new ASTTypeVariable();

      AntlrNode typenode = node.getChild(index);
      switch ( typenode.getType() )
      {
         case TYPEINFO:
            break;

         case EXTENDS:
            {
               ptype->setMode(ASTTypeVariable::eExtends);
               AntlrNode idnode = typenode.getChild(2);
               ptype->setOther(getType(idnode));
            }
            break;

         case SUPER:
            {
               ptype->setMode(ASTTypeVariable::eSuper);
               AntlrNode idnode = typenode.getChild(2);
               ptype->setOther(getType(idnode));
            }
            break;
      }

      AntlrNode idnode = typenode.getChild(0);
      ptype->setIdentifier(idnode.toString());

      ptypes->append(ptype);
   }

   return ptypes;
}

ASTNode* AntlrParser::handleClass(const AntlrNode& node)
{
   ASTClass* pclass = new ASTClass();
   mpClass = pclass;

   if ( node.getType() == INTRFACE )
      pclass->setKind(ASTClass::eInterface);
   else
      pclass->setKind(ASTClass::eClass);

   int count = node.getChildCount();

   AntlrNode modsnode = node.getChild(0);
   handleModifiers(modsnode, pclass->getModifiers());

   AntlrNode namenode = node.getChild(1);
   String name = namenode.toString();
   String qualifiedname = mClassResolver.resolve(name);

   pclass->setName(name);
   pclass->setPosition(namenode.getPosition());
   pclass->setFullName(qualifiedname);

   if ( count > 2 )
   {
      int index = 2;

      // check for type info
      AntlrNode anode = node.getChild(index);
      if ( anode.getType() == TYPEINFOS )
      {
         ASTTypeVariables* ptypes = handleTypeVariables(anode);
         pclass->setTypeVariables(ptypes);
         index++;
      }

      // check base class
      AntlrNode member = node.getChild(index);
      int nodetype = member.getType();
      if ( nodetype == EXTENDS )
      {
         AntlrNode typenode = node.getChild(index+1);
         pclass->setBaseType(getType(typenode));

         index += 2; // extends & id
      }

      // check for implementing interfaces
      member = node.getChild(index);
      if ( !member.isNil() )
      {
         // if nil, then there are also no members
         nodetype = member.getType();
         if ( nodetype == IMPL )
         {
            AntlrNode typelistnode = node.getChild(index + 1);
            int listsize = typelistnode.getChildCount();
            for ( int i = 0; i < listsize; i++ )
            {
               AntlrNode typenode = typelistnode.getChild(i);
               pclass->addInterface(getType(typenode));
            }

            index += 2;
         }

         // iterate members
         for ( ; index < count; index++ )
         {
            AntlrNode member = node.getChild(index);

            ASTMember* pmember = dynamic_cast<ASTMember*>(handleTree(member));
            ASSERT_PTR(pmember);

            pclass->addMember(pmember);
         }
      }
   }

   return pclass;
}

ASTMember* AntlrParser::handleVarDecl(const AntlrNode& node)
{
   ASTVariable* pvariable = new ASTVariable();
   pvariable->setLocation(ASTVariable::eField);

   AntlrNode modnode = node.getChild(0);
   handleModifiers(modnode, pvariable->getModifiers());

   AntlrNode typenode = node.getChild(1);
   pvariable->setType(getType(typenode));

   AntlrNode namenode = node.getChild(2);
   pvariable->setName(namenode.toString());

   if ( node.getChildCount() > 3 )
   {
      // skip the '='
      AntlrNode initnode = node.getChild(4);
      pvariable->setInit(handleVarInit(initnode));      
   }

   return new ASTField(pvariable);
}

ASTMember* AntlrParser::handlePropertyDecl(const AntlrNode& node)
{
   ASTProperty* pproperty = new ASTProperty();

   AntlrNode typenode = node.getChild(0);
   pproperty->setType(getType(typenode));

   AntlrNode namenode = node.getChild(1);
   pproperty->setName(namenode.toString());

   return pproperty;
}

void AntlrParser::handleFuncArguments(const AntlrNode& node, ASTFunction& function)
{
   ASSERT(node.getType() == FUNCTION_ARGUMENTS);

   int count = node.getChildCount();
   int index = 0;

   while ( index < count )
   {
      AntlrNode typenode = node.getChild(index);

      ASTVariable* pvariable = new ASTVariable();
      pvariable->setLocation(ASTVariable::eArgument);
      pvariable->setPosition(typenode.getPosition());      
      pvariable->setType(getType(typenode));

      AntlrNode namenode = node.getChild(index+1);
      pvariable->setName(namenode.toString());

      ASTFunctionArgument* pargument = new ASTFunctionArgument(pvariable);
      pargument->setPosition(typenode.getPosition());

      function.addArgument(pargument);

      index += 2;
   }
}

ASTMember* AntlrParser::handleConstructor(const AntlrNode& node)
{
   ASTFunction* pfunction = new ASTFunction(ASTMember::eConstructor);
   pfunction->setType(ASTType::SVoidType.clone()); // <-- invalid type, constructors do not have return types
   pfunction->setPosition(node.getPosition());

   AntlrNode modnode = node.getChild(0);
   handleModifiers(modnode, pfunction->getModifiers());

   AntlrNode namenode = node.getChild(1);
   pfunction->setName(namenode.toString());

   AntlrNode arguments = node.getChild(2);
   handleFuncArguments(arguments, *pfunction);

   AntlrNode block = node.getChild(3);
   ANTLR3_UINT32 nodetype = block.getType();
   if ( nodetype == BLOCK )
   {
      ASTBlock* pbody = handleBlock(block);
      pfunction->setBody(pbody);
   }
   else
   {
      ASSERT(nodetype == SEP);
      if ( pfunction->getModifiers().isNative() )
      {
         pfunction->getModifiers().setPureNative();
      }
   }

   return pfunction;
}

ASTMember* AntlrParser::handleInterfaceMember(const AntlrNode& node)
{
   ASTModifiers modifiers;
   AntlrNode modnode = node.getChild(0);
   handleModifiers(modnode, modifiers);
   modifiers.setAbstract();
   modifiers.setVisibility(ASTModifiers::ePublic);

   AntlrNode typenode = node.getChild(1);
   ASTType* ptype = getType(typenode);

   AntlrNode namenode = node.getChild(2);
   String name = namenode.toString();

   AntlrNode argumentnode = node.getChild(3);
   int type = argumentnode.getType();
   if ( type == FUNCTION_ARGUMENTS )
   {
      ASTFunction* pfunction = new ASTFunction(ASTMember::eFunction);
      pfunction->setPosition(namenode.getPosition());
      pfunction->setModifiers(modifiers);
      pfunction->setType(ptype);
      pfunction->setName(name);

      handleFuncArguments(argumentnode, *pfunction);

      return pfunction;
   }
   else
   {
      ASTVariable* pvariable = new ASTVariable();
      pvariable->setPosition(namenode.getPosition());
      pvariable->setLocation(ASTVariable::eField);
      pvariable->setModifiers(modifiers);
      pvariable->setType(ptype);
      pvariable->setName(name);

      if ( type != SEP )
      {
         AntlrNode exprnode = node.getChild(4);
         pvariable->setInit(handleVarInit(exprnode));
      }

      return new ASTField(pvariable);
   }
}

ASTMember* AntlrParser::handleInterfaceVoidMember(const AntlrNode& node)
{
   ASTModifiers modifiers;
   AntlrNode modnode = node.getChild(0);
   handleModifiers(modnode, modifiers);
   modifiers.setAbstract();
   modifiers.setVisibility(ASTModifiers::ePublic);

   AntlrNode namenode = node.getChild(1);
   String name = namenode.toString();

   AntlrNode argumentnode = node.getChild(2);
   int type = argumentnode.getType();
   if ( type == FUNCTION_ARGUMENTS )
   {
      ASTFunction* pfunction = new ASTFunction(ASTMember::eFunction);
      pfunction->setPosition(namenode.getPosition());
      pfunction->setModifiers(modifiers);
      pfunction->setType(ASTType::SVoidType.clone());
      pfunction->setName(name);

      handleFuncArguments(argumentnode, *pfunction);

      return pfunction;
   }
   else
   {
      UNREACHABLE("Void type member variables are not supported");
      return nullptr;
      /*
      ASTVariable* pvariable = new ASTVariable();
      pvariable->setLocation(ASTVariable::eField);
      pvariable->setModifiers(modifiers);
      pvariable->setType(ASTType::SVoidType.clone());
      pvariable->setName(name);

      if ( type != SEP )
      {
         AntlrNode exprnode = node.getChild(3);
         pvariable->setInit(handleVarInit(exprnode));
      }

      return new ASTField(pvariable);
      */
   }
}

ASTMember* AntlrParser::handleFuncDecl(const AntlrNode& node)
{
   ASTFunction* pfunction = new ASTFunction(ASTMember::eFunction);

   AntlrNode annotationnode = node.getChild(0);
   pfunction->setAnnotations(dynamic_cast<ASTAnnotations*>(handleTree(annotationnode)));

   AntlrNode modnode = node.getChild(1);
   handleModifiers(modnode, pfunction->getModifiers());

   int index = 2;
   AntlrNode typevarnode = node.getChild(index);
   if ( typevarnode.getType() == TYPEINFOS )
   {
      ASTTypeVariables* ptypes = handleTypeVariables(typevarnode);
      pfunction->setTypeVariables(ptypes);

      index++;
   }

   AntlrNode typenode = node.getChild(index++);
   pfunction->setType(getType(typenode));

   AntlrNode namenode = node.getChild(index++);
   pfunction->setName(namenode.toString());
   pfunction->setPosition(typenode.getPosition());

   AntlrNode arguments = node.getChild(index++);
   handleFuncArguments(arguments, *pfunction);

   AntlrNode block = node.getChild(index++);
   ANTLR3_UINT32 nodetype = block.getType();
   if ( nodetype == BLOCK )
   {
      ASTBlock* pbody = handleBlock(block);
      pfunction->setBody(pbody);
   }
   else
   {
      ASSERT(nodetype == SEP);
      if ( pfunction->getModifiers().isNative() )
      {
         pfunction->getModifiers().toPureNative();
      }
   }

   return pfunction;
}

ASTMember* AntlrParser::handleVoidFuncDecl(const AntlrNode& node)
{
   ASTFunction* pfunction = new ASTFunction(ASTMember::eFunction);
   pfunction->setType(ASTType::SVoidType.clone());

   AntlrNode annotationnode = node.getChild(0);
   pfunction->setAnnotations(dynamic_cast<ASTAnnotations*>(handleTree(annotationnode)));

   AntlrNode modnode = node.getChild(1);
   handleModifiers(modnode, pfunction->getModifiers());

   int index = 2;
   AntlrNode typenode = node.getChild(index);
   if ( typenode.getType() == TYPEINFOS )
   {
      ASTTypeVariables* ptypes = handleTypeVariables(typenode);
      pfunction->setTypeVariables(ptypes);

      index++;
   }

   AntlrNode namenode = node.getChild(index++);
   pfunction->setName(namenode.toString());
   pfunction->setPosition(namenode.getPosition());

   AntlrNode arguments = node.getChild(index++);
   handleFuncArguments(arguments, *pfunction);

   AntlrNode block = node.getChild(index++);
   int nodetype = block.getType();
   if ( nodetype == BLOCK )
   {
      ASTBlock* pbody = handleBlock(block);
      pfunction->setBody(pbody);
   }
   else
   {
      ASSERT(nodetype == SEP);
      if ( pfunction->getModifiers().isNative() )
      {
         pfunction->getModifiers().toPureNative();
      }
   }

   return pfunction;
}

ASTAnnotations* AntlrParser::handleAnnotations(const AntlrNode& node)
{
   int count = node.getChildCount();
   ASTAnnotations* pannotations = nullptr;

   if ( count > 0 )
   {
      pannotations = new ASTAnnotations();

      for ( int index = 0; index < count; index++ )
      {
         AntlrNode child = node.getChild(index);
         ASSERT(child.getType() == ANNOTATION);

         AntlrNode namenode = child.getChild(0);
         ASTAnnotation* pannotation = new ASTAnnotation;
         pannotation->mName = namenode.toString();

         pannotations->add(pannotation);
      }
   }

   return pannotations;
}

ASTArrayInit* AntlrParser::handleArrayInit(const AntlrNode& node)
{
   ASTArrayInit* parrayinit = new ASTArrayInit();
   parrayinit->setPosition(node.getPosition());

   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode child = node.getChild(index);
      ASTVariableInit* pinit = handleVarInit(child);
      parrayinit->addChild(pinit);
   }

   return parrayinit;
}

ASTVariableInit* AntlrParser::handleVarInit(const AntlrNode& node)
{
   ASTVariableInit* pinit = new ASTVariableInit();
   pinit->setPosition(node.getPosition());

   if ( node.getType() == ARRAYINIT )
   {
      ASTArrayInit* parrayinit = handleArrayInit(node);
      pinit->setArrayInit(parrayinit);
   }
   else
   {
      ASSERT(node.getType() == EXPRESSION);
      ASTExpression* pexpression = handleExpression(node);
      pinit->setExpression(pexpression);
   }

   return pinit;
}

ASTBlock* AntlrParser::handleBlock(const AntlrNode& node)
{
   ASSERT(node.getType() == BLOCK);

   ASTBlock* pblock = new ASTBlock();

   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode member = node.getChild(index);
      ASTNode* pnode = handleTree(member);
      ASSERT_PTR(pnode);

      ASTStatement* pstatement = dynamic_cast<ASTStatement*>(pnode);
      pblock->addChild(pstatement);
   }

   return pblock;
}

ASTLocalVariable* AntlrParser::handleLocalVarDecl(const AntlrNode& node)
{
   AntlrNode typenode = node.getChild(0);
   AntlrNode namenode = node.getChild(1);

   ASTVariable* pvariable = new ASTVariable();
   pvariable->setName(namenode.toString());
   pvariable->setPosition(namenode.getPosition());
   pvariable->setLocation(ASTVariable::eLocal);
   pvariable->setType(getType(typenode));
   
   int count = node.getChildCount();
   if ( count > 2 )
   {
      AntlrNode initnode = node.getChild(2);
      pvariable->setInit(handleVarInit(initnode));
   }

   return new ASTLocalVariable(pvariable);
}

ASTIf* AntlrParser::handleIf(const AntlrNode& node)
{
   ASTIf* pif = new ASTIf();
   pif->setPosition(node.getPosition());

   AntlrNode conditionnode = node.getChild(0);
   pif->setCondition(handleCompound(conditionnode));

   AntlrNode statement = node.getChild(1);
   ASTStatement* pstatement = dynamic_cast<ASTStatement*>(handleTree(statement));
   ASSERT_PTR(pstatement);
   pif->setStatement(pstatement);

   int count = node.getChildCount();
   if ( count > 2 )
   {
      statement = node.getChild(2);
      pstatement = dynamic_cast<ASTStatement*>(handleTree(statement));
      ASSERT_PTR(pstatement);
      pif->setElseStatement(pstatement);
   }

   return pif;
}

ASTFor* AntlrParser::handleFor(const AntlrNode& node)
{
   ASTFor* pfor = new ASTFor();
   pfor->setPosition(node.getPosition());

   AntlrNode stmtnode = node.getChild(0);
   ASTStatement* pstatement = dynamic_cast<ASTStatement*>(handleTree(stmtnode));
   ASSERT_PTR(pstatement);
   pfor->setBody(pstatement);

   AntlrNode child = node.getChild(1);
   int nodetype = child.getType();

   int index = 2;
   if ( nodetype != SEP )
   {
      pfor->setInitializer(handleLocalVarDecl(child));

      index++;
   }

   child = node.getChild(index);

   if ( child.getType() != SEP )
   {
      pfor->setCondition(handleExpression(child));

      index++;
   }

   int count = node.getChildCount();
   if ( count >= index )
   {
      for ( index++ ; index < count; index += 2 ) // skip the comma of expression list
      {
         child = node.getChild(index);

         pfor->addLoop(handleExpression(child));
      }
   }

   return pfor;
}

ASTForeach* AntlrParser::handleForeach(const AntlrNode& node)
{
   ASTVariable* pvariable = new ASTVariable();
   pvariable->setLocation(ASTVariable::eLocal);

   AntlrNode typenode = node.getChild(0);
   pvariable->setType(getType(typenode));

   AntlrNode namenode = node.getChild(1);
   pvariable->setName(namenode.toString());
   pvariable->setPosition(namenode.getPosition());

   AntlrNode exprnode = node.getChild(2);
   pvariable->setInit(handleVarInit(exprnode));

   AntlrNode statement = node.getChild(3);
   ASTStatement* pstatement = dynamic_cast<ASTStatement*>(handleTree(statement));
   ASSERT_PTR(pstatement);

   ASTForeach* pforeach = new ASTForeach();
   pforeach->setPosition(node.getPosition());
   pforeach->setVariable(pvariable);
   pforeach->setBody(pstatement);

   return pforeach;
}

ASTWhile* AntlrParser::handleWhile(const AntlrNode& node)
{
   ASTWhile* pwhile = new ASTWhile();
   pwhile->setPosition(node.getPosition());

   AntlrNode condition = node.getChild(0);
   pwhile->setCondition(handleCompound(condition));

   AntlrNode body = node.getChild(1);
   ASTStatement* pstatement = dynamic_cast<ASTStatement*>(handleTree(body));
   ASSERT_PTR(pstatement);
   pwhile->setBody(pstatement);

   return pwhile;
}

ASTDo* AntlrParser::handleDo(const AntlrNode& node)
{
   ASTDo* pdo = new ASTDo();
   pdo->setPosition(node.getPosition());

   AntlrNode body = node.getChild(0);
   ASTStatement* pstatement = dynamic_cast<ASTStatement*>(handleTree(body));
   ASSERT_PTR(pstatement);
   pdo->setBody(pstatement);

   AntlrNode condition = node.getChild(1);
   pdo->setCondition(handleCompound(condition));

   return pdo;
}

ASTSwitch* AntlrParser::handleSwitch(const AntlrNode& node)
{
   ASTSwitch* pswitch = new ASTSwitch();
   pswitch->setPosition(node.getPosition());

   AntlrNode condition = node.getChild(0);
   ASTNode* pexpr = handleTree(condition);
   ASSERT_PTR(pexpr);
   pswitch->setExpression(pexpr);

   int count = node.getChildCount();
   for ( int index = 1; index < count; index++ )
   {
      AntlrNode child = node.getChild(index);
      ASTCase* pcase = dynamic_cast<ASTCase*>(handleTree(child));
      ASSERT_PTR(pcase);

      pswitch->addChild(pcase);
   }

   return pswitch;
}

ASTCase* AntlrParser::handleCase(const AntlrNode& node)
{
   ASTCase* pcase = new ASTCase();
   pcase->setPosition(node.getPosition());

   int bodyindex = 0;
   if ( node.getType() == CASE )
   {
      pcase->setKind(ASTCase::eCase);

      AntlrNode valuenode = node.getChild(0);
      ASTUnary* pnode = handleUnary(valuenode);
      pcase->setValueExpression(pnode);

      bodyindex++;
   }
   else // default
   {
      pcase->setKind(ASTCase::eDefault);
   }

   ASTNode* pbody = nullptr;
   AntlrNode bodynode = node.getChild(bodyindex);
   if ( node.getChildCount() == (bodyindex + 1) )
   {
      pbody = handleTree(bodynode);
      if ( bodynode.getType() != BLOCK )
      {
         ASTBlock* pblock = new ASTBlock();
         pblock->addChild(pbody);

         pbody = pblock;
      }
   }
   else
   {
      pbody = new ASTBlock();
      for ( int index = bodyindex; index < node.getChildCount(); index++ )
      {
         bodynode = node.getChild(index);
         ASTNode* pnode = handleTree(bodynode);
         pbody->addChild(pnode);
      }
   }

   pcase->setBody(pbody); // <-- always a block

   return pcase;
}

ASTReturn* AntlrParser::handleReturn(const AntlrNode& node)
{
   ASTReturn* preturn = new ASTReturn();
   preturn->setPosition(node.getPosition());

   if ( node.getChildCount() == 1 )
   {
      AntlrNode expression = node.getChild(0);
      preturn->setExpression(handleExpression(expression));
   }

   return preturn;
}

ASTTry* AntlrParser::handleTry(const AntlrNode& node)
{
   ASTTry* ptry = new ASTTry();
   ptry->setPosition(node.getPosition());

   AntlrNode blocknode = node.getChild(0);
   ptry->setBody(handleBlock(blocknode));

   int count = node.getChildCount();
   for ( int index = 1; index < count; index++ )
   {
      AntlrNode child = node.getChild(index);
      switch ( child.getType() )
      {
         case CATCH:
            {
               ptry->addCatch(handleCatch(child));
            }
            break;
         case FINALLY:
            {
               AntlrNode blocknode = child.getChild(0);
               ptry->setFinallyBlock(handleBlock(blocknode));
            }
            break;
         default:
            break;
      }
   }

   return ptry;
}

ASTCatch* AntlrParser::handleCatch(const AntlrNode& node)
{
   ASTVariable* pvariable = new ASTVariable();
   pvariable->setLocation(ASTVariable::eLocal);

   AntlrNode typenode = node.getChild(0);
   pvariable->setType(getType(typenode));

   AntlrNode namenode = node.getChild(1);
   pvariable->setName(namenode.toString());
   pvariable->setPosition(namenode.getPosition());

   AntlrNode bodynode = node.getChild(2);

   ASTCatch* pcatch = new ASTCatch();
   pcatch->setVariable(new ASTLocalVariable(pvariable));
   pcatch->setPosition(node.getPosition());
   pcatch->setBody(handleBlock(bodynode));

   return pcatch;
}

ASTThrow* AntlrParser::handleThrow(const AntlrNode& node)
{
   AntlrNode child = node.getChild(0);

   ASTThrow* pthrow = new ASTThrow();
   pthrow->setPosition(node.getPosition());
   pthrow->setExpression(handleExpression(child));
   return pthrow;
}

ASTAssert* AntlrParser::handleAssert(const AntlrNode& node)
{
   AntlrNode child = node.getChild(0);

   ASTAssert* passert = new ASTAssert();
   passert->setPosition(node.getPosition());
   passert->setCondition(handleExpression(child));

   return passert;
}

ASTLoopControl* AntlrParser::handleLoopControl(const AntlrNode& node)
{
   ASTLoopControl* pcontrol = new ASTLoopControl(node.getType() == BREAK ? ASTLoopControl::eBreak : ASTLoopControl::eContinue);
   pcontrol->setPosition(node.getPosition());
   return pcontrol;
}

ASTExpressionStatement* AntlrParser::handleExpressionStatement(const AntlrNode& node)
{
   AntlrNode exprnode = node.getChild(0);
   ASTExpression* pexpression = dynamic_cast<ASTExpression*>(handleTree(exprnode));
   ASSERT_PTR(pexpression);

   ASTExpressionStatement* pstatement = new ASTExpressionStatement();
   pstatement->setPosition(exprnode.getPosition());
   pstatement->setExpression(pexpression);
   return pstatement;
}

ASTCompound* AntlrParser::handleCompound(const AntlrNode& node)
{
   AntlrNode comp = node.getChild(0);
   ASTExpression* pexpression = dynamic_cast<ASTExpression*>(handleTree(comp));

   ASTCompound* pnode = new ASTCompound();
   pnode->setPosition(node.getPosition());
   pnode->setExpression(pexpression);

   return pnode;
}

ASTExpression* AntlrParser::handleExpression(const AntlrNode& node)
{
   ASSERT(node.getType() == EXPRESSION);

   ASTExpression* pexpression = new ASTExpression();
   pexpression->setPosition(node.getPosition());

   AntlrNode unarynode = node.getChild(0);
   pexpression->setLeft(handleTree(unarynode));

   int count = node.getChildCount();
   if ( count > 1 )
   {
      AntlrNode operatornode = node.getChild(1);
      String op = operatornode.toString();

      if ( op == UTEXT("=") )
      {
         pexpression->setKind(ASTExpression::eAssign);
      }
      else if ( op == UTEXT("+=") )
      {
         pexpression->setKind(ASTExpression::ePlusAssign);
      }
      else if ( op == UTEXT("-=") )
      {
         pexpression->setKind(ASTExpression::eMinAssign);
      }

      AntlrNode rightnode = node.getChild(2);
      ASTExpression* pright = handleExpression(rightnode);
      pexpression->setRight(pright);
   }
   else
   {
      pexpression->setKind(ASTExpression::eUnary);
   }

   return pexpression;
}

ASTNode* AntlrParser::handleConcatenate(const AntlrNode& node)
{
   AntlrNode expr = node.getChild(0);
   ASTNode* presult = handleTree(expr);

   int count = node.getChildCount();
   if ( count == 2 )
   {
      AntlrNode right = node.getChild(1);
      ASTNode* pright = handleTree(right);

      ASTConcatenate::Mode mode = ASTConcatenate::eInvalid;
      switch ( node.getType() )
      {
         case MUL:
            mode = ASTConcatenate::eMul;
            break;
         case DIV:
            mode = ASTConcatenate::eDiv;
            break;
         case REM:
            mode = ASTConcatenate::eRem;
            break;
         case PLUS:
            mode = ASTConcatenate::ePlus;
            break;
         case MINUS:
            mode = ASTConcatenate::eMinus;
            break;
         case AND:
            mode = ASTConcatenate::eAnd;
            break;
         case OR:
            mode = ASTConcatenate::eOr;
            break;
         case BITOR:
            mode = ASTConcatenate::eBitwiseOr;
            break;
         case BITXOR:
            mode = ASTConcatenate::eBitwiseXor;
            break;
         case BITAND:
            mode = ASTConcatenate::eBitwiseAnd;
            break;
         case SHIFTL:
            mode = ASTConcatenate::eShiftLeft;
            break;
         case SHIFTR:
            mode = ASTConcatenate::eShiftRight;
            break;
         case EQUAL:
            mode = ASTConcatenate::eEquals;
            break;
         case UNEQUAL:
            mode = ASTConcatenate::eUnequals;
            break;
         case SEQUAL:
            mode = ASTConcatenate::eSmallerEqual;
            break;
         case SMALLER:
            mode = ASTConcatenate::eSmaller;
            break;
         case GREATER:
            mode = ASTConcatenate::eGreater;
            break;
         case GEQUAL:
            mode = ASTConcatenate::eGreaterEqual;
            break;
      }

      ASTConcatenate* pconcat = new ASTConcatenate(mode);
      pconcat->setPosition(node.getPosition());
      pconcat->setLeft(presult);
      pconcat->setRight(pright);

      presult = pconcat;
   }

   return presult;
}

ASTNode* AntlrParser::handleInstanceOf(const AntlrNode& node)
{
   ASTInstanceOf* pinstanceof = new ASTInstanceOf();
   pinstanceof->setPosition(node.getPosition());

   AntlrNode left = node.getChild(0);
   ASTNode* pnode = handleTree(left);
   pinstanceof->setObject(pnode);

   AntlrNode right = node.getChild(1);
   ASTType* ptype = getType(right);
   pinstanceof->setInstanceType(ptype);

   return pinstanceof;
}

ASTUnary::Operator handleUnaryOperator(const AntlrNode& node)
{
   ASTUnary::Operator op = ASTUnary::eNone;
   switch ( node.getType() )
   {
      case PLUS:
         op = ASTUnary::ePos;
         break;
      case MINUS:
         op = ASTUnary::eNegate;
         break;
      case NOT:
         op = ASTUnary::eNot;
         break;
      case INCR:
         op = ASTUnary::eIncrement;
         break;
      case DECR:
         op = ASTUnary::eDecrement;
         break;
   }

   return op;
}

ASTUnary* AntlrParser::handleUnary(const AntlrNode& node)
{
   ASTUnary* punary = new ASTUnary();
   punary->setPosition(node.getPosition());

   int index = 0;

   // check pre-operator
   AntlrNode child = node.getChild(0);
   ASTUnary::Operator op = handleUnaryOperator(child);
   if ( op != ASTUnary::eNone )
   {
      punary->setPre(op);
      index++;
   }

   int count = node.getChildCount();
   for ( ; index < count - 1; index++ )
   {
      child = node.getChild(index);

      ASTExpressionPart* ppart = dynamic_cast<ASTExpressionPart*>(handleTree(child));
      ASSERT_PTR(ppart);

      punary->addPart(ppart);
   }

   // check post-operator
   child = node.getChild(index);
   op = handleUnaryOperator(child);
   if ( op != ASTUnary::eNone )
   {
      punary->setPost(op);
   }
   else
   {
      ASTExpressionPart* ppart = dynamic_cast<ASTExpressionPart*>(handleTree(child));
      ASSERT_PTR(ppart);

      punary->addPart(ppart);
   }

   return punary;
}

ASTNew* AntlrParser::handleNew(const AntlrNode& node)
{
   ASTNew* pnew = new ASTNew();
   pnew->setPosition(node.getPosition());

   AntlrNode typenode = node.getChild(0);
   ASTType* ptype = getType(typenode);

   AntlrNode argsnode = node.getChild(1);
   int argstype = argsnode.getType();
   if ( argstype == ARGUMENTS )
   {
      pnew->setKind(ASTNew::eObject);
      int count = argsnode.getChildCount();

      for ( int index = 0; index < count; index += 2 )  // <-- skip the ,
      {
         AntlrNode argnode = argsnode.getChild(index);
         ASTExpression* parg = handleExpression(argnode);

         pnew->addArgument(parg);
      }
   }
   else if ( argstype == ARRAYARGUMENTS )
   {
      // make it an array type
      ASTType* parraytype = new ASTType(ASTType::eArray);
      parraytype->setPosition(argsnode.getPosition());
      parraytype->setArrayDimension(1);
      parraytype->setArrayType(ptype);
      ptype = parraytype;

      AntlrNode argnode = argsnode.getChild(0);
      ASTExpression* parg = handleExpression(argnode);

      pnew->setKind(ASTNew::eArray);
      pnew->addArgument(parg);
   }
   else
   {
      UNREACHABLE("Invalid type for new");
   }

   pnew->setType(ptype);

   return pnew;
}

ASTNode* AntlrParser::handleSuper(const AntlrNode& node)
{
   ASTSuper* psuper = new ASTSuper();
   psuper->setPosition(node.getPosition());
      
   switch ( node.getType() )
   {
   case SUPER:
      psuper->setKind(ASTSuper::eSuper);
      break;
   case THIS:
      psuper->setKind(ASTSuper::eThis);
      break;
   }

   int count = node.getChildCount();
   if ( count == 1 )
   {
      psuper->setCall(true);

      AntlrNode argsnode = node.getChild(0);
      ASSERT(argsnode.getType() == ARGUMENTS);

      count = argsnode.getChildCount();
      for ( int index = 0; index < count; index += 2 ) // skip the ,
      {
         AntlrNode argnode = argsnode.getChild(index);
         ASTExpression* parg = handleExpression(argnode);

         psuper->addChild(parg);
      }
   }

   return psuper;
}

ASTNode* AntlrParser::handleNative(const AntlrNode& node)
{
   ASTNative* pnative = new ASTNative();
   pnative->setPosition(node.getPosition());

   int count = node.getChildCount();
   if ( count == 1 )
   {
      AntlrNode argsnode = node.getChild(0);
      ASSERT(argsnode.getType() == ARGUMENTS);

      count = argsnode.getChildCount();
      for ( int index = 0; index < count; index += 2 ) // skip the ,
      {
         AntlrNode argnode = argsnode.getChild(index);
         ASTExpression* parg = handleExpression(argnode);

         pnative->addChild(parg);
      }
   }

   return pnative;
}

ASTCast* AntlrParser::handleCast(const AntlrNode& node)
{
   ASTCast* pcast = new ASTCast();
   pcast->setPosition(node.getPosition());

   AntlrNode typenode = node.getChild(0);
   pcast->setType(getType(typenode));

   AntlrNode expr = node.getChild(1);
   pcast->setNode(handleTree(expr));

   return pcast;
}

ASTAccess* AntlrParser::handleAccess(const AntlrNode& node)
{
   ASTAccess* paccess = new ASTAccess();
   paccess->setPosition(node.getPosition());

   int count = node.getChildCount();

   AntlrNode namenode = node.getChild(0);
   paccess->setName(namenode.toString());
   paccess->setKind(count == 1 ? ASTAccess::eVariable : ASTAccess::eFunction);

   if ( count > 1 )
   {
      AntlrNode argsnode = node.getChild(1);
      if ( argsnode.getType() == TYPEARGUMENT )
      {
         int typearguments = argsnode.getChildCount();
         for ( int index = 0; index < typearguments; index++ )
         {
            AntlrNode typeargument = argsnode.getChild(index);

            ASTType* pargumenttype = getType(typeargument);

            paccess->getTypeArguments().append(pargumenttype);
         }

         argsnode = node.getChild(2);
      }

      count = argsnode.getChildCount();

      for ( int index = 0; index < count; index += 2 )  // <-- skip the () and ,
      {
         AntlrNode argnode = argsnode.getChild(index);
         ASTExpression* parg = handleExpression(argnode);

         paccess->addArgument(parg);
      }
   }

   return paccess;
}

ASTAccess* AntlrParser::handleArrayAccess(const AntlrNode& node)
{
   ASTAccess* paccess = new ASTAccess();
   paccess->setPosition(node.getPosition());
   paccess->setKind(ASTAccess::eArray);

   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode argnode = node.getChild(index);
      ASTExpression* parg = handleExpression(argnode);

      paccess->addArgument(parg);
   }

   return paccess;
}

ASTAccess* AntlrParser::handleClassAccess(const AntlrNode& node)
{
   ASTAccess* paccess = new ASTAccess();
   paccess->setPosition(node.getPosition());
   paccess->setKind(ASTAccess::eClass);
   return paccess;
}

ASTLiteral* AntlrParser::handleLiteral(const AntlrNode& node)
{
   AntlrNode valuenode = node.getChild(0);
   String valuestr = valuenode.toString();

   Variant value;
   ASTType::Kind kind = ASTType::eInt;
   ANTLR3_UINT32 valuetype = valuenode.getType();
   switch ( valuetype )
   {
      case INT:
         kind = ASTType::eInt;
         value.setInt(NumberConverter::getInstance().toInt(valuestr));
         break;
      case FLOAT:
         kind = ASTType::eReal;
         value.setReal(NumberConverter::getInstance().toFloat(valuestr));
         break;
      case CHAR:
         kind = ASTType::eChar;
         value.setChar(parseChar(valuestr));
         break;
      case STRING:
         kind = ASTType::eString;
         value.setString(parseString(valuestr));
         break;
      case LITTRUE:
         kind = ASTType::eBoolean;
         value.setBool(true);
         break;
      case LITFALSE:
         kind = ASTType::eBoolean;
         value.setBool(false);
         break;
      case LITNULL:
         kind = ASTType::eNull;
         break;
      default:
         break;
   }

   Literal* pliteral = mContext.getLiteralTable().find(value);
   if ( pliteral == nullptr )
   {
      pliteral = new Literal();
      pliteral->setValue(value);

      mContext.getLiteralTable().insert(pliteral);
   }

   ASSERT(pliteral->getTableIndex() >= 0);

   ASTLiteral* pastliteral = new ASTLiteral(*pliteral, kind);
   pastliteral->setPosition(node.getPosition());
   return pastliteral;
}

// - Helpers

UChar AntlrParser::parseChar(const String& value)
{
   ASSERT(value[0] == '\'');

   //String unquoted = value.subStr(1, value.length() - 2);
   
   return value.unescape()[1];
}

String AntlrParser::parseString(const String& value)
{
   ASSERT(value[0] == '\"');

   String unquoted = value.subStr(1, value.length() - 2);   
   return unquoted.unescape();
}

// - Error reporting

// This method is taken from antlr3baserecognizer.c and adapted to throw an exception
// instread of reporting it to the console.

static void reportError(pANTLR3_BASE_RECOGNIZER recognizer, pANTLR3_UINT8 * tokenNames)
{
   pANTLR3_EXCEPTION	ex = recognizer->state->exception;

   if ( ex->streamName == nullptr )
   {
      if ( ((pANTLR3_COMMON_TOKEN)(ex->token))->type == ANTLR3_TOKEN_EOF )
      {
         // ANTLR3_FPRINTF(stderr, "-end of input-(");
      }
      else
      {
         //ANTLR3_FPRINTF(stderr, "-unknown source-(");
      }
   }

   int linenr = ex->line;
   int charpos = ex->charPositionInLine;
   String message;

   switch ( ex->type )
   {
      case ANTLR3_UNWANTED_TOKEN_EXCEPTION:

         // Indicates that the recognizer was fed a token which seesm to be
		   // spurious input. We can detect this when the token that follows
		   // this unwanted token would normally be part of the syntactically
		   // correct stream. Then we can see that the token we are looking at
		   // is just something that should not be there and throw this exception.
		   //
         if ( tokenNames == nullptr )
         {
            message = UTEXT("Extraneous input...");
         }
         else
         {
            if	(ex->expecting == ANTLR3_TOKEN_EOF)
			   {
				   message = UTEXT("Extraneous input - expected <EOF>");
			   }
			   else
			   {
               message = UTEXT("Extraneous input - expected ") + String::fromUtf8((const char*)tokenNames[ex->expecting]);
			   }
         }
         break;
      case ANTLR3_MISSING_TOKEN_EXCEPTION:

         // Indicates that the recognizer detected that the token we just
		   // hit would be valid syntactically if preceeded by a particular
		   // token. Perhaps a missing ';' at line end or a missing ',' in an
		   // expression list, and such like.
		   //
         if ( tokenNames == nullptr )
         {
            message = UTEXT("Missing token...");
         }
         else
         {
            if	(ex->expecting == ANTLR3_TOKEN_EOF)
			   {
				   message = UTEXT("Missing <EOF>");
			   }
			   else
			   {
               message = UTEXT("Missing ") + String::fromUtf8((const char*)tokenNames[ex->expecting]);
			   }
         }
         break;

      case ANTLR3_RECOGNITION_EXCEPTION:

         // Indicates that the recognizer received a token
		   // in the input that was not predicted. This is the basic exception type
		   // from which all others are derived. So we assume it was a syntax error.
		   // You may get this if there are not more tokens and more are needed
		   // to complete a parse for instance.
		   //
         message = UTEXT("Syntax error...");
         break;

      case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:

         // We were expecting to see one thing and got another. This is the
		   // most common error if we coudl not detect a missing or unwanted token.
		   // Here you can spend your efforts to
		   // derive more useful error messages based on the expected
		   // token set and the last token and so on. The error following
		   // bitmaps do a good job of reducing the set that we were looking
		   // for down to something small. Knowing what you are parsing may be
		   // able to allow you to be even more specific about an error.
		   //
         if ( tokenNames == nullptr )
         {
            message = UTEXT("Syntax error...");
         }
         else
         {
            if	(ex->expecting == ANTLR3_TOKEN_EOF)
			   {
				   message = UTEXT("Expected <EOF>");
			   }
			   else
			   {
               message = UTEXT("Expected ") + String::fromUtf8((const char*)tokenNames[ex->expecting]);
			   }
         }
         break;
      case ANTLR3_NO_VIABLE_ALT_EXCEPTION:

         // We could not pick any alt decision from the input given
		   // so god knows what happened - however when you examine your grammar,
		   // you should. It means that at the point where the current token occurred
		   // that the DFA indicates nowhere to go from here.
		   //
         message = UTEXT("Can not match to any predicted input");
         break;
      case ANTLR3_MISMATCHED_SET_EXCEPTION:
         {
			   ANTLR3_UINT32	  count;
			   ANTLR3_UINT32	  bit;
			   ANTLR3_UINT32	  size;
			   ANTLR3_UINT32	  numbits;
			   pANTLR3_BITSET	  errBits;

			   // This means we were able to deal with one of a set of
			   // possible tokens at this point, but we did not see any
			   // member of that set.
			   //
			   message = UTEXT("Unexpected input.\nExpected one of : ");

			   // What tokens could we have accepted at this point in the
			   // parse?
			   //
			   count   = 0;
			   errBits = antlr3BitsetLoad		(ex->expectingSet);
			   numbits = errBits->numBits		(errBits);
			   size    = errBits->size			(errBits);

			   if  (size > 0)
			   {
				   // However many tokens we could have dealt with here, it is usually
				   // not useful to print ALL of the set here. I arbitrarily chose 8
				   // here, but you should do whatever makes sense for you of course.
				   // No token number 0, so look for bit 1 and on.
				   //
				   for	(bit = 1; bit < numbits && count < 8 && count < size; bit++)
				   {
					   // TODO: This doesn;t look right - should be asking if the bit is set!!
					   //
					   if  (tokenNames[bit])
					   {
                     message += String(count > 0 ? ", " : "") + String::fromUtf8((const char*)tokenNames[bit]);
						   count++;
					   }
				   }
			   }
			   else
			   {
				   message = UTEXT("Actually dude, we didn't seem to be expecting anything here, or at least\n")
				           + UTEXT("I could not work out what I was expecting, like so many of us these days!");
			   }
		   }
         break;

      case ANTLR3_EARLY_EXIT_EXCEPTION:

         // We entered a loop requiring a number of token sequences
		   // but found a token that ended that sequence earlier than
		   // we should have done.
		   //
         message = UTEXT("Missing elements...");
         break;
      default:

         // We don't handle any other exceptions here, but you can
		   // if you wish. If we get an exception that hits this point
		   // then we are just going to report what we know about the
		   // token.
		   //

         message = UTEXT("Syntax not recognised.");
         break;
   }

   throw new AntlrException(message, linenr, charpos);
}
