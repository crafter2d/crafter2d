
#ifndef STRING_CACHE_H
#define STRING_CACHE_H

class StringData;

class StringCache
{
public:
   StringCache();



private:
   typedef std::vector<StringData*> Strings;

   Strings mStrings;
};

#endif // STRING_CACHE_H
