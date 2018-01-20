
#include "effectfileparser.h"

#include "core/string/char.h"
#include "core/string/string.h"
#include "core/defines.h"

#include "lexer.h"

#include "astannotation.h"
#include "asteffect.h"
#include "astbuffer.h"
#include "astdefine.h"
#include "astfunction.h"
#include "astfunctionargument.h"
#include "astsampler.h"
#include "aststruct.h"
#include "aststructentry.h"
#include "asttechnique.h"
#include "asttexture.h"
#include "asttype.h"

static const String sTechnique(UTEXT("technique"));
static const String sStruct(UTEXT("struct"));
static const String sBuffer(UTEXT("cbuffer"));
static const String sDefine(UTEXT("#define"));
static const String sTexture(UTEXT("texture"));
static const String sSampler(UTEXT("sampler"));
static const String sLanguage(UTEXT("language"));
static const String sPoint(UTEXT("point"));
static const String sInOut(UTEXT("inout"));

EffectFileParser::EffectFileParser():
   mpEffect(NULL)
{
}

ASTEffect* EffectFileParser::parse(const String& content)
{
   Lexer lexer(content);

   ASTEffect* presult = parseDeclarations(lexer);

   return presult;
}

ASTEffect* EffectFileParser::parseDeclarations(Lexer& lexer)
{
   mpEffect = new ASTEffect();

   while ( !lexer.isEOF() )
   {
      if ( lexer.next(sStruct) )
      {
         ASTStruct* pstruct = parseStruct(lexer);
         mpEffect->addStruct(pstruct);
      }
      else if ( lexer.next(sBuffer) )
      {
         ASTBuffer* pbuffer = parseBuffer(lexer);
         mpEffect->addBuffer(pbuffer);
      }
      else if ( lexer.next(sTexture) )
      {
         ASTTexture* ptexture = parseTexture(lexer);
         mpEffect->addTexture(ptexture);
      }
      else if ( lexer.next(sSampler) )
      {
         ASTSampler* psampler = parseSampler(lexer);
         mpEffect->addSampler(psampler);
      }
      else if (lexer.next(sDefine))
      {
         ASTDefine* pdefine = parseDefine(lexer);
         mpEffect->addDefine(pdefine);
      }
      else if ( lexer.next(sLanguage) )
      {
         String lang = lexer.getToken();
         lexer.next(L';');

         if ( lang == UTEXT("dx") )
         {
            mpEffect->setLanguage(ASTEffect::eDirectX);
         }
         else if ( lang == UTEXT("ogl") )
         {
            mpEffect->setLanguage(ASTEffect::eOpenGL);
         }
      }
      else if ( lexer.next(sTechnique) )
      {
         ASTTechnique *ptechnique = parseTechnique(lexer);
         mpEffect->addTechnique(ptechnique);
      }
      else
      {
         ASTFunction* pfunction = parseFunction(lexer);
         mpEffect->addFunction(pfunction);
      }
   }

   if ( !mpEffect->hasSupportedLanguage() )
   {
      determineLanguage();
   }

   return mpEffect;
}

ASTStruct* EffectFileParser::parseStruct(Lexer& lexer)
{
   ASTStruct* presult = new ASTStruct();
   presult->mName = lexer.getIdentifier();
   
   if ( !lexer.next(L'{') )
   {
      throw std::logic_error("{ expected.");
   }

   while ( !lexer.next('}') )
   {
      ASTStructEntry* pentry = parseStructEntry(lexer);
      presult->mEntries.push_back(pentry);
   }

   lexer.next(';');

   return presult;
}

ASTStructEntry* EffectFileParser::parseStructEntry(Lexer& lexer)
{
   ASTStructEntry* pentry = new ASTStructEntry();

   String typestr = lexer.getIdentifier();
   pentry->ptype = ASTType::fromString(*mpEffect, typestr);
   pentry->name  = lexer.getIdentifier();

   if ( lexer.next(L':') )
   {
      pentry->location = lexer.getIdentifier();
      UChar ch = pentry->location[pentry->location.length() - 1];
      if ( Char::isDigit(ch) )
      {
         pentry->slot = ch;
         pentry->location = pentry->location.subStr(0, pentry->location.length() - 1);
      }
   }

   if ( !lexer.next(L';') )
   {
      String errormsg = String("; expected at line {0} col {1}.").arg(0, lexer.getLine()).arg(1, lexer.getColumn());
      throw std::logic_error(errormsg.toUtf8());
   }

   return pentry;
}

ASTBuffer* EffectFileParser::parseBuffer(Lexer& lexer)
{
   ASTBuffer* presult = new ASTBuffer();
   presult->mName = lexer.getIdentifier();

   if ( lexer.next(':') )
   {
      presult->mRegister = lexer.getNumber();
   }

   presult->mBody = readStructBody(lexer);
   if ( presult->mBody.isEmpty() )
   {
      // meh!
      throw std::logic_error(("could not parse the body of struct " + presult->mName.toUtf8()).c_str());
   }

   lexer.next(';');

   return presult;
}

ASTTechnique* EffectFileParser::parseTechnique(Lexer& lexer)
{
   ASTTechnique* presult = new ASTTechnique();
   presult->mName = lexer.getIdentifier();

   if ( !lexer.next('{') )
   {
      throw std::logic_error(("expected a block opener for technique " + presult->mName.toUtf8()).c_str());
   }

   while ( !lexer.next('}') )
   {
      String token = lexer.getToken();
      lexer.next('=');
      if ( token == UTEXT("vertex") )
      {
         presult->mVertex.mTarget = lexer.getIdentifier();
         presult->mVertex.mEntry = lexer.getIdentifier();
      }
      else if ( token == UTEXT("geometry") )
      {
         presult->mGeometry.mTarget = lexer.getIdentifier();
         presult->mGeometry.mEntry = lexer.getIdentifier();
      }
      else if ( token == UTEXT("pixel") )
      {
         presult->mPixel.mTarget = lexer.getIdentifier();
         presult->mPixel.mEntry = lexer.getIdentifier();
      }
      else
      {
         throw std::logic_error(("Invalid shader type in technique " + presult->mName.toUtf8()).c_str());
      }
   }

   return presult;
}

ASTTexture* EffectFileParser::parseTexture(Lexer& lexer)
{
   ASTTexture* presult = new ASTTexture();
   presult->mName = lexer.getIdentifier();

   if ( lexer.next(L':') )
   {
      presult->mRegister = lexer.getNumber();
   }

   lexer.next(L';');

   return presult;
}

ASTSampler* EffectFileParser::parseSampler(Lexer& lexer)
{
   ASTSampler* presult = new ASTSampler();
   presult->mName = lexer.getIdentifier();

   if ( lexer.next(L':') )
   {
      presult->mRegister = lexer.getNumber();
   }

   lexer.next(L';');

   return presult;
}

ASTDefine* EffectFileParser::parseDefine(Lexer& lexer)
{
   ASTDefine* pdefine = new ASTDefine();
   pdefine->mName = lexer.getIdentifier();
   pdefine->mValue = lexer.getNumber();
   return pdefine;
}

ASTType* EffectFileParser::parseType(Lexer& lexer)
{
   String tmpl;
   String type = lexer.getIdentifier();
   if ( lexer.next(L'<') )
   {
      tmpl = lexer.getIdentifier();
      lexer.next(L'>');
   }

   ASTType* ptype = ASTType::fromString(*mpEffect, tmpl.isEmpty() ? type : tmpl);
   if ( ptype == NULL || ptype->isUnknown() )
   {
      throw std::logic_error(("Invalid token: " + type.toUtf8()).c_str());
   }

   if ( !tmpl.isEmpty() )
   {
      ptype->setTemplateClass(type);
   }

   return ptype;
}

ASTFunction* EffectFileParser::parseFunction(Lexer& lexer)
{
   ASTFunction* pfunc = new ASTFunction();
   parseAnnotations(lexer, *pfunc);

   pfunc->mpType = parseType(lexer);
   pfunc->mName = lexer.getIdentifier();
   
   parseArguments(lexer, *pfunc);

   if ( lexer.next(':') )
   {
      pfunc->mReturn = lexer.getIdentifier();
   }

   pfunc->mBody = readFunctionBody(lexer);

   return pfunc;
}

void EffectFileParser::parseArguments(Lexer& lexer, ASTFunction& func)
{
   if ( !lexer.next(L'(') )
   {
      throw std::logic_error("Function arguments expected!");
   }

   if ( !lexer.next(L')') )
   {
      do
      {
         ASTFunctionArgument* parg = parseArgument(lexer);
         func.addArgument(parg);
      } while ( lexer.next(L',') );

      lexer.next(L')');
   }
}

ASTFunctionArgument* EffectFileParser::parseArgument(Lexer& lexer)
{
   ASTFunctionArgument* parg = new ASTFunctionArgument();
   parg->mpType = parseType(lexer);
   parg->mName = lexer.getIdentifier();

   if ( lexer.next(L'[') )
   {
      parg->mArraySize = lexer.getNumber();

      lexer.next(L']');
   }

   return parg;
}

void EffectFileParser::parseAnnotations(Lexer& lexer, ASTFunction& function)
{
   if ( lexer.next(L'[') )
   {
      do
      {
         ASTAnnotation* panno = parseAnnotation(lexer);
         if ( panno != NULL )
         {
            function.addAnnotation(panno);
         }
      }
      while ( lexer.next(L',') && !lexer.isEOF() );

      lexer.next(L']');
   }
}

ASTAnnotation* EffectFileParser::parseAnnotation(Lexer& lexer)
{
   ASTAnnotation* panno = new ASTAnnotation();
   panno->name = lexer.getIdentifier();

   if ( lexer.next(L'(') )
   {
      String value = lexer.getIdentifier();
      if ( value.isEmpty() )
      {
         int number = lexer.getNumber();
         panno->setValue(number);
      }
      else
      {
         panno->setValue(value);
      }

      lexer.next(L')');
   }

   return panno;
}

String EffectFileParser::readStructBody(Lexer& lexer)
{
   String body;
   int depth = 1;

   if ( lexer.next(L'{') )
   {
      while ( true )
      {
         UChar ch = lexer.getChar();
         if ( ch == L'}' )
         {
            depth--;
            if ( depth == 0 )
            {
               break;
            }
         }
         else if ( ch == L'{' )
         {
            depth++;
         }

         body += ch;
      }

      lexer.next(L'}');
   }
   return body;
}

String EffectFileParser::readFunctionBody(Lexer& lexer)
{
   String body;

   if ( lexer.next(L'{') )
   {
      int depth = 0;
      while ( true )
      {
         UChar ch = lexer.getChar();
         if ( ch == L'{' )
         {
            depth++;
         }
         else if ( ch == L'}' )
         {
            if ( depth == 0 )
            {
               break;
            }
            depth--;
         }

         body += ch;
      }

      lexer.next(L'}');
   }
   return body;
}


// - Helpers

void EffectFileParser::determineLanguage()
{
   ASSERT_PTR(mpEffect);

   if ( !mpEffect->mFunctions.empty() )
   {
      ASTTechnique* ptechnique = mpEffect->mTechniques[0];
      const ASTFunction* pfunction = mpEffect->findFunction(ptechnique->mVertex.mEntry);

      if ( pfunction != NULL )
      {
         String identifiers[] = { UTEXT("gl_"), UTEXT("vec2"), UTEXT("mat4") };
         int count = sizeof(identifiers) / sizeof(String);

         for ( int index = 0; index < count; ++index )
         {
            std::size_t pos = pfunction->mBody.indexOf(identifiers[index]);
            if ( pos != String::npos )
            {
               mpEffect->setLanguage(ASTEffect::eOpenGL);
               return;
            }
         }

         mpEffect->setLanguage(ASTEffect::eDirectX);
         return;
      }
   }

   throw new std::logic_error("No language was specified, and could not determine the language automatically.");
}
