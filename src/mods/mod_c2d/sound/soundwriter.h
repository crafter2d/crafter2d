
#ifndef SOUND_WRITER_H
#define SOUND_WRITER_H

#include "core/content/contentwriter.h"

class SoundWriter : public c2d::ContentWriter
{
public:

 // overrides
   virtual bool write(DataStream& stream, const String& filename) override;

};

#endif // SOUND_WRITER_H
