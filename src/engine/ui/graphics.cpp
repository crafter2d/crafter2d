
#include "graphics.h"

#include <GL/GLee.h>

Graphics::Graphics()
{
}

void Graphics::fillRect(int x, int y, int width, int height)
{
   glRectf(x, y, x+width, y+height);
}

void Graphics::drawText(int x, int y, const std::string& text)
{
}

void Graphics::translate(float x, float y)
{
   glTranslatef(x, y, 0);
}

void Graphics::setColor(float r, float g, float b, float a)
{
   glColor4f(r, g, b, a);
}
