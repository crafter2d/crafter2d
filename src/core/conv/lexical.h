
#ifndef LEXICAL_H_
#define LEXICAL_H_

#include <sstream>

template<typename Target, typename Source>
class Lexer
{
public:
   Lexer() {}

   bool operator<<(const Source& source) {
      mStream << source;
      return true;
   }

   bool operator>>(Target& target) {
      mStream >> target;
      return true;
   }

private:
   std::stringstream mStream;
};

template<typename Target, typename Source>
Target lexical_cast(const Source& source)
{
   Lexer<Target, Source> lexer;
   lexer << source;

   Target t;
   lexer >> t;

   return t;
}

#endif // LEXICAL_H_
