
#ifndef SOUND_READER_H
#define SOUND_READER_H

#include "core/content/contentreader.h"

class SoundReader : public c2d::ContentReader
{
public:

 // overrides
   virtual IContent* read(DataStream& stream) override;

};

#endif // SOUND_READER_H
