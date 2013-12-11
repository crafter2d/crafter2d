
#ifndef EFFECT_FILE_PARSER_H
#define EFFECT_FILE_PARSER_H

class ASTEffect;
class ASTBuffer;
class ASTStruct;
class ASTStructEntry;
class ASTFunction;
class ASTFunctionArgument;
class ASTSampler;
class ASTTechnique;
class ASTTexture;
class Lexer;
class String;

class EffectFileParser
{
public:
   EffectFileParser();

   ASTEffect* parse(const String& filename);

private:

 // parsing
   ASTEffect*           parseDeclarations(Lexer& lexer);
   ASTStruct*           parseStruct(Lexer& lexer);
   ASTStructEntry*      parseStructEntry(Lexer& lexer);
   ASTBuffer*           parseBuffer(Lexer& lexer);
   ASTFunction*         parseFunction(Lexer& lexer, const String& type);
   ASTFunctionArgument* parseArgument(Lexer& lexer);
   ASTTechnique*        parseTechnique(Lexer& lexer);
   ASTTexture*          parseTexture(Lexer& lexer);
   ASTSampler*          parseSampler(Lexer& lexer);

   void parseArguments(Lexer& lexer, ASTFunction& func);

   String readStructBody(Lexer& lexer);
   String readFunctionBody(Lexer& lexer);

 // helpers
   void determineLanguage();

 // data
   ASTEffect* mpEffect;
};

#endif // EFFECT_FILE_PARSER_H
