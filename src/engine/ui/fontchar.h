
#ifndef FONT_CHAR_H_
#define FONT_CHAR_H_

class FontChar
{
public:
   FontChar(int left, int top, int width, int height, int advancex, int advancey, unsigned char* pdata);
   ~FontChar();

 // rendering
   void render();

private:
   friend class Font;

   int            mLeft;
   int            mTop;
   int            mWidth;
   int            mHeight;
   int            mAdvanceX;
   int            mAdvanceY;
   unsigned char* mpBitmap;
};

#endif // FONT_CHAR_H_
