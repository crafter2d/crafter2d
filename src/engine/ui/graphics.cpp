
#include "graphics.h"

#include <GL/GLee.h>

#include "core/defines.h"

#include "font.h"

Graphics::Graphics():
   mpFont(NULL)
{
}

void Graphics::fillRect(int x, int y, int width, int height)
{
   glRectf(x, y, x+width, y+height);
}

void Graphics::drawText(int x, int y, const std::string& text)
{
   ASSERT_PTR(mpFont);

   glRasterPos2i(x, y);

   mpFont->render(text);
}

void Graphics::translate(float x, float y)
{
   glTranslatef(x, y, 0);
}

void Graphics::setFont(Font& font)
{
   mpFont = &font;
}

void Graphics::setColor(float r, float g, float b, float a)
{
   glColor4f(r, g, b, a);
}
