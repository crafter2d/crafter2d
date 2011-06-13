
#include "fontchar.h"

#include <GL/GLee.h>

FontChar::FontChar(int left, int top, int width, int height, int advancex, int advancey, unsigned char* pdata):
   mLeft(left),
   mTop(top),
   mWidth(width),
   mHeight(height),
   mAdvanceX(advancex),
   mAdvanceY(advancey),
   mpBitmap(pdata)
{
}

FontChar::~FontChar()
{
   delete[] mpBitmap;
   mpBitmap = NULL;
}

// - Rendering

void FontChar::render()
{
   glBitmap(mWidth, mHeight, mLeft, mTop, mAdvanceX, mAdvanceY, mpBitmap);
}
