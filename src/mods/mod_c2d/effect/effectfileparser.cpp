
#include "effectfileparser.h"

#include "core/string/char.h"
#include "core/string/string.h"
#include "core/defines.h"

#include "lexer.h"

#include "asteffect.h"
#include "astbuffer.h"
#include "astfunction.h"
#include "astfunctionargument.h"
#include "astsampler.h"
#include "aststruct.h"
#include "aststructentry.h"
#include "asttechnique.h"
#include "asttexture.h"
#include "asttype.h"

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
      String token = lexer.getToken();

      if ( token.isEmpty() )
      {
         // we are done
         break;
      }

      if ( token == UTEXT("struct") )
      {
         ASTStruct* pstruct = parseStruct(lexer);
         mpEffect->addStruct(pstruct);
      }
      else if ( token == UTEXT("cbuffer") )
      {
         ASTBuffer* pbuffer = parseBuffer(lexer);
         mpEffect->addBuffer(pbuffer);
      }
      else if ( token == UTEXT("texture") )
      {
         ASTTexture* ptexture = parseTexture(lexer);
         mpEffect->addTexture(ptexture);
      }
      else if ( token == UTEXT("sampler") )
      {
         ASTSampler* psampler = parseSampler(lexer);
         mpEffect->addSampler(psampler);
      }
      else if ( token == UTEXT("technique") )
      {
         ASTTechnique *ptechnique = parseTechnique(lexer);
         mpEffect->addTechnique(ptechnique);
      }
      else if ( token == UTEXT("language") )
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
      else
      {
         ASTFunction* pfunction = parseFunction(lexer, token);
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
      throw std::exception("{ expected.");
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
      throw std::exception("; expected");
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
      throw std::exception(("could not parse the body of struct " + presult->mName.toUtf8()).c_str());
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
      throw std::exception(("expected a block opener for technique " + presult->mName.toUtf8()).c_str());
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
      else if ( token == UTEXT("pixel") )
      {
         presult->mPixel.mTarget = lexer.getIdentifier();
         presult->mPixel.mEntry = lexer.getIdentifier();
      }
      else
      {
         throw std::exception(("Invalid shader type in technique " + presult->mName.toUtf8()).c_str());
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

   return presult;
}

ASTFunction* EffectFileParser::parseFunction(Lexer& lexer, const String& type)
{
   ASTType* ptype = ASTType::fromString(*mpEffect, type);
   if ( ptype == NULL || ptype->isUnknown() )
   {
      throw std::exception(("Invalid token: " + type.toUtf8()).c_str());
   }

   ASTFunction* pfunc = new ASTFunction();
   pfunc->mName = lexer.getIdentifier();
   pfunc->mpType = ptype;

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
      throw std::exception("Function arguments expected!");
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
   parg->mpType = ASTType::fromString(*mpEffect, lexer.getIdentifier());
   parg->mName = lexer.getIdentifier();
   return parg;
}

String EffectFileParser::readStructBody(Lexer& lexer)
{
   String body;

   if ( lexer.next(L'{') )
   {
      while ( true )
      {
         UChar ch = lexer.getChar();
         if ( ch == L'}' )
         {
            break;
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
            int pos = pfunction->mBody.indexOf(identifiers[index]);
            if ( pos != -1 )
            {
               mpEffect->setLanguage(ASTEffect::eOpenGL);
               return;
            }
         }

         mpEffect->setLanguage(ASTEffect::eDirectX);
         return;
      }
   }

   throw new std::exception("No language was specified, and could not determine the language automatically.");
}
