
#include "antlrinterface.h"

// undefine the 'emit' define of Qt as ANTLR needs it
#undef emit

#include "core/defines.h"
#include "core/conv/lexical.h"

#include "script/output/asLexer.h"
#include "script/output/asParser.h"
#include "script/ast/astannotation.h"
#include "script/compiler/compilecontext.h"
#include "script/common/literal.h"
#include "script/common/variant.h" 

#include "antlrnode.h"
#include "antlrstream.h"

AntlrParser::AntlrParser(CompileContext& context):
   mContext(context)
{
}

// - Parse

ASTRoot* AntlrParser::parse(const AntlrStream& stream)
{
   pasLexer lexer = asLexerNew(stream.getStream());
   if ( lexer == NULL )
   {
      mContext.getLog().error("failed to instantiate lexer");
      return 0;
   }

   pANTLR3_COMMON_TOKEN_STREAM tstream;
   tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
   if ( tstream == NULL )
   {
      mContext.getLog().error("can't open token stream");
      lexer->free(lexer);
      return 0;
   }

   pasParser parser = asParserNew(tstream);
   if ( parser == NULL )
   {
      mContext.getLog().error("failed to instantiate parser");
      lexer->free(lexer);
      tstream->free(tstream);
      return 0;
   }

   ASTRoot* proot = NULL;
   asParser_script_return ast = parser->script(parser);
   if ( parser->pParser->rec->state->errorCount > 0 )
   {
      mContext.getLog().error("errors while parsing input");
   }
   else
   {
      proot = new ASTRoot();

      AntlrNode root(ast.tree);

      if ( !root.isNil() )
      {
         proot->addChild(handleTree(root));
      }
      else
      {
         int count = root.getChildCount();
         for ( int index = 0; index < count; index++ )
         {
            AntlrNode declnode = root.getChild(index);
            proot->addChild(handleTree(declnode));
         }
      }
   }

   parser->free(parser);
   tstream->free(tstream);
   lexer->free(lexer);

   return proot;
}

ASTType* AntlrParser::getType(const AntlrNode& node)
{
   ASSERT(node.getType() == TYPE);

   AntlrNode typenode = node.getChild(0);

   ASTType* ptype = new ASTType();

   int nodetype = typenode.getType();
   switch ( nodetype )
   {
      case T_INT:
         ptype->setKind(ASTType::eInt);
         break;
      case T_REAL:
         ptype->setKind(ASTType::eReal);
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
            std::string name = typenode.toString();

            ptype->setKind(ASTType::eObject);
            ptype->setObjectName(name);
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
      case PACKAGE:           return handlePackage(node);
      case USE:               return handleUse(node);
      case CLASS:             return handleClass(node);
      case INTRFACE:          return handleClass(node);
      case IFACE_MEMBER:      return handleInterfaceMember(node);
      case VARIABLE_DECL:     return handleVarDecl(node);
      case CONSTRUCTOR_DECL:  return handleConstructor(node);
      case FUNCTION_DECL:     return handleFuncDecl(node);
      case VOID_FUNCDECL:     return handleVoidFuncDecl(node);
      case ANNOTATIONS:       return handleAnnotations(node);
      case BLOCK:             return handleBlock(node);
      case LOCALVARDECL:      return handleLocalVarDecl(node);
      case IF:                return handleIf(node);
      case FOR:               return handleFor(node);
      case FOREACH:           return handleForeach(node);
      case WHILE:             return handleWhile(node);
      case DO:                return handleDo(node);
      case RETURN:            return handleReturn(node);
      case TRY:               return handleTry(node);
      case THROW:             return handleThrow(node);
      case STMT_EXPR:         return handleExpressionStatement(node);
      case COMPOUNT:          return handleCompound(node);
      case EXPRESSION:        return handleExpression(node);
      case UNARY:             return handleUnary(node);
      case NEW:               return handleNew(node);
      case SUPER:             return handleSuper(node);
      case THIS:              return handleSuper(node);
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

   return NULL;
}

ASTNode* AntlrParser::handlePackage(const AntlrNode& node)
{
   std::string identifier;
   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode namenode = node.getChild(index);
      identifier += namenode.toString();

      if ( index < count - 1 )
      {
         identifier += '\\';
      }
   }

   ASTPackage* ppackage = new ASTPackage();
   ppackage->setName(identifier);

   return ppackage;
}

ASTNode* AntlrParser::handleUse(const AntlrNode& node)
{
   std::string identifier;
   int count = node.getChildCount();
   for ( int index = 0; index < count; index++ )
   {
      AntlrNode namenode = node.getChild(index);
      identifier += namenode.toString();

      if ( index < count - 1 )
      {
         identifier += '/';
      }
   }

   ASTUse* puse = new ASTUse();
   puse->setIdentifier(identifier);
   
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
      std::string id, extra;
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
   pclass->setName(namenode.toString());

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

   //mContext.addClass(pclass);

   return pclass;
}

ASTMember* AntlrParser::handleVarDecl(const AntlrNode& node)
{
   ASTVariable* pvariable = new ASTVariable();

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
      ASTExpression* pexpression = handleExpression(initnode);
      pvariable->setExpression(pexpression);
   }

   return new ASTField(pvariable);
}

void AntlrParser::handleFuncArguments(const AntlrNode& node, ASTFunction& function)
{
   ASSERT(node.getType() == FUNCTION_ARGUMENTS);

   int count = node.getChildCount();
   int index = 0;

   while ( index < count )
   {
      ASTVariable* pvariable = new ASTVariable();
      AntlrNode typenode = node.getChild(index);
      pvariable->setType(getType(typenode));

      AntlrNode namenode = node.getChild(index+1);
      pvariable->setName(namenode.toString());

      ASTFunctionArgument* pargument = new ASTFunctionArgument(pvariable);
      function.addArgument(pargument);

      index += 2;
   }
}

ASTMember* AntlrParser::handleConstructor(const AntlrNode& node)
{
   ASTFunction* pfunction = new ASTFunction(ASTMember::eConstructor);
   pfunction->setType(&ASTType::SVoidType); // <-- invalid type, constructors do not have return types

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
      ASSERT(nodetype == SEP);

   return pfunction;
}

ASTMember* AntlrParser::handleInterfaceMember(const AntlrNode& node)
{
   ASTModifiers modifiers;
   AntlrNode modnode = node.getChild(0);
   handleModifiers(modnode, modifiers);

   AntlrNode typenode = node.getChild(1);
   ASTType* ptype = getType(typenode);

   AntlrNode namenode = node.getChild(2);
   std::string name = namenode.toString();

   AntlrNode argumentnode = node.getChild(3);
   int type = argumentnode.getType();
   if ( type == FUNCTION_ARGUMENTS )
   {
      ASTFunction* pfunction = new ASTFunction(ASTMember::eFunction);
      pfunction->setModifiers(modifiers);
      pfunction->setType(ptype);
      pfunction->setName(name);

      handleFuncArguments(argumentnode, *pfunction);

      return pfunction;
   }
   else
   {
      ASTVariable* pvariable = new ASTVariable();
      pvariable->setModifiers(modifiers);
      pvariable->setType(ptype);
      pvariable->setName(name);
      
      if ( type != SEP )
      {
         AntlrNode exprnode = node.getChild(4);
         ASTExpression* pexpression = dynamic_cast<ASTExpression*>(handleTree(exprnode));
         ASSERT_PTR(pexpression);

         pvariable->setExpression(pexpression);
      }

      return new ASTField(pvariable);
   }
}

ASTMember* AntlrParser::handleFuncDecl(const AntlrNode& node)
{
   ASTFunction* pfunction = new ASTFunction(ASTMember::eFunction);

   AntlrNode annotationnode = node.getChild(0);
   pfunction->setAnnotations(dynamic_cast<ASTAnnotations*>(handleTree(annotationnode)));

   AntlrNode modnode = node.getChild(1);
   handleModifiers(modnode, pfunction->getModifiers());

   AntlrNode typenode = node.getChild(2);
   pfunction->setType(getType(typenode));
               
   AntlrNode namenode = node.getChild(3);
   pfunction->setName(namenode.toString());

   AntlrNode arguments = node.getChild(4);
   handleFuncArguments(arguments, *pfunction);

   AntlrNode block = node.getChild(5);
   ANTLR3_UINT32 nodetype = block.getType();
   if ( nodetype == BLOCK )
   {
      ASTBlock* pbody = handleBlock(block);
      pfunction->setBody(pbody);
   }
   else
      ASSERT(nodetype == SEP);
               
   return pfunction;
}

ASTMember* AntlrParser::handleVoidFuncDecl(const AntlrNode& node)
{
   ASTFunction* pfunction = new ASTFunction(ASTMember::eFunction);
   pfunction->setType(&ASTType::SVoidType);

   AntlrNode annotationnode = node.getChild(0);
   pfunction->setAnnotations(dynamic_cast<ASTAnnotations*>(handleTree(annotationnode)));

   AntlrNode modnode = node.getChild(1);
   handleModifiers(modnode, pfunction->getModifiers());
              
   AntlrNode namenode = node.getChild(2);
   pfunction->setName(namenode.toString());

   AntlrNode arguments = node.getChild(3);
   handleFuncArguments(arguments, *pfunction);

   AntlrNode block = node.getChild(4);
   int nodetype = block.getType();
   if ( nodetype == BLOCK )
   {
      ASTBlock* pbody = handleBlock(block);
      pfunction->setBody(pbody);
   }
   else
      ASSERT(nodetype == SEP);
               
   return pfunction;
}

ASTAnnotations* AntlrParser::handleAnnotations(const AntlrNode& node)
{
   int count = node.getChildCount();
   ASTAnnotations* pannotations = NULL;

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
   ASTVariable* pvariable = new ASTVariable();

   AntlrNode typenode = node.getChild(0);
   pvariable->setType(getType(typenode));
               
   AntlrNode namenode = node.getChild(1);
   pvariable->setName(namenode.toString());

   int count = node.getChildCount();
   if ( count > 2 )
   {
      AntlrNode initnode = node.getChild(2);
      ASTExpression* pinit = handleExpression(initnode);

      pvariable->setExpression(pinit);
   }

   return new ASTLocalVariable(pvariable);
}

ASTIf* AntlrParser::handleIf(const AntlrNode& node)
{
   ASTIf* pif = new ASTIf();

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

   AntlrNode typenode = node.getChild(0);
   pvariable->setType(getType(typenode));

   AntlrNode namenode = node.getChild(1);
   pvariable->setName(namenode.toString());

   AntlrNode exprnode = node.getChild(2);
   ASTExpression* pexpression = dynamic_cast<ASTExpression*>(handleTree(exprnode));
   ASSERT_PTR(pexpression);
   pvariable->setExpression(pexpression);

   AntlrNode statement = node.getChild(3);
   ASTStatement* pstatement = dynamic_cast<ASTStatement*>(handleTree(statement));
   ASSERT_PTR(pstatement);

   ASTForeach* pforeach = new ASTForeach();
   pforeach->setVariable(pvariable);
   pforeach->setBody(pstatement);

   return pforeach;
}

ASTWhile* AntlrParser::handleWhile(const AntlrNode& node)
{
   ASTWhile* pwhile = new ASTWhile();

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

   AntlrNode body = node.getChild(0);
   ASTStatement* pstatement = dynamic_cast<ASTStatement*>(handleTree(body));
   ASSERT_PTR(pstatement);
   pdo->setBody(pstatement);

   AntlrNode condition = node.getChild(1);
   pdo->setCondition(handleCompound(condition));

   return pdo;
}

ASTReturn* AntlrParser::handleReturn(const AntlrNode& node)
{
   ASTReturn* preturn = new ASTReturn();

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
   
   AntlrNode typenode = node.getChild(0);
   pvariable->setType(getType(typenode));
               
   AntlrNode namenode = node.getChild(1);
   pvariable->setName(namenode.toString());

   AntlrNode bodynode = node.getChild(2);
   
   ASTCatch* pcatch = new ASTCatch();
   pcatch->setVariable(new ASTLocalVariable(pvariable));
   pcatch->setBody(handleBlock(bodynode));

   return pcatch;
}

ASTThrow* AntlrParser::handleThrow(const AntlrNode& node)
{
   AntlrNode child = node.getChild(0);

   ASTThrow* pthrow = new ASTThrow();
   pthrow->setExpression(handleExpression(child));
   return pthrow;
}

ASTLoopControl* AntlrParser::handleLoopControl(const AntlrNode& node)
{
   ASTLoopControl* pcontrol = new ASTLoopControl(node.getType() == BREAK ? ASTLoopControl::eBreak : ASTLoopControl::eContinue);
   return pcontrol;
}

ASTExpressionStatement* AntlrParser::handleExpressionStatement(const AntlrNode& node)
{
   AntlrNode exprnode = node.getChild(0);
   ASTExpression* pexpression = dynamic_cast<ASTExpression*>(handleTree(exprnode));
   ASSERT_PTR(pexpression);

   ASTExpressionStatement* pstatement = new ASTExpressionStatement();
   pstatement->setExpression(pexpression);
   return pstatement;
}

ASTCompound* AntlrParser::handleCompound(const AntlrNode& node)
{
   AntlrNode comp = node.getChild(0);
   ASTExpression* pexpression = dynamic_cast<ASTExpression*>(handleTree(comp));

   ASTCompound* pnode = new ASTCompound();
   pnode->setExpression(pexpression);
               
   return pnode;
}

ASTExpression* AntlrParser::handleExpression(const AntlrNode& node)
{
   ASSERT(node.getType() == EXPRESSION);

   ASTExpression* pexpression = new ASTExpression();

   AntlrNode unarynode = node.getChild(0);
   pexpression->setLeft(handleTree(unarynode));

   int count = node.getChildCount();
   if ( count > 1 )
   {
      AntlrNode operatornode = node.getChild(1);
      std::string op = operatornode.toString();

      if ( op == "=" )
      {
         pexpression->setKind(ASTExpression::eAssign);
      }
      else if ( op == "+=" )
      {
         pexpression->setKind(ASTExpression::ePlusAssign);
      }
      else if ( op == "-=" )
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
      pconcat->setLeft(presult);
      pconcat->setRight(pright);

      presult = pconcat;
   }

   return presult;
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
   psuper->setKind(node.getType() == SUPER ? ASTSuper::eSuper : ASTSuper::eThis);

   int count = node.getChildCount();
   if ( count == 1 )
   {
      psuper->setCall(true);

      AntlrNode argsnode = node.getChild(0);
      count = argsnode.getChildCount();
      for ( int index = 0; index < count; index++ )
      {
         AntlrNode argnode = argsnode.getChild(index);
         ASTExpression* parg = handleExpression(argnode);

         psuper->addChild(parg);
      }
   }

   return psuper;
}

ASTCast* AntlrParser::handleCast(const AntlrNode& node)
{
   ASTCast* pcast = new ASTCast();

   AntlrNode typenode = node.getChild(0);
   pcast->setType(getType(typenode));

   AntlrNode expr = node.getChild(1);
   pcast->setNode(handleTree(expr));

   return pcast;
}

ASTAccess* AntlrParser::handleAccess(const AntlrNode& node)
{
   ASTAccess* paccess = new ASTAccess();
   int count = node.getChildCount();

   AntlrNode namenode = node.getChild(0);
   paccess->setName(namenode.toString());
   paccess->setKind(count == 1 ? ASTAccess::eVariable : ASTAccess::eFunction);
   
   if ( count == 2 )
   {
      AntlrNode argsnode = node.getChild(1);
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
   paccess->setKind(ASTAccess::eClass);
   return paccess;
}

ASTLiteral* AntlrParser::handleLiteral(const AntlrNode& node)
{
   int count = node.getChildCount();
   AntlrNode valuenode = node.getChild(0);
   std::string valuestr = valuenode.toString();

   Variant value;
   ASTType::Kind kind = ASTType::eInt;
   ANTLR3_UINT32 valuetype = valuenode.getType();
   switch ( valuetype )
   {
      case INT:
         kind = ASTType::eInt;
         value.setInt(lexical_cast<int>(valuestr));
         break;
      case FLOAT:
         kind = ASTType::eReal;
         value.setReal(lexical_cast<double>(valuestr));
         break;
      case STRING:
         kind = ASTType::eString;
         value.setString(valuestr.substr(1, valuestr.length() - 2)); // cut the "";
         break;
      case LITTRUE:
         kind = ASTType::eBoolean;
         value.setBool(true);
         break;
      case LITFALSE:
         kind = ASTType::eBoolean;
         value.setBool(false);
         break;
      default:
         break;
   }
   
   Literal* pliteral = mContext.getLiteralTable().find(value);
   if ( pliteral == NULL )
   {
      pliteral = new Literal();
      pliteral->setValue(value);
      
      mContext.getLiteralTable().insert(pliteral);
   }

   ASSERT(pliteral->getTableIndex() >= 0);

   ASTLiteral* pastliteral = new ASTLiteral(*pliteral, kind);
   return pastliteral;
}
