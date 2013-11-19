
#ifndef EFFECT_INPUT_LAYOUT_H
#define EFFECT_INPUT_LAYOUT_H

#include <vector>

#include "core/string/string.h"

class EffectInputLayout
{
public:

   struct Entry
   {
      int    index;
      int    size;
      String semantic;
   };

   typedef std::vector<Entry> Entries;

   Entries entries;
};

#endif // EFFECT_INPUT_LAYOUT_H
