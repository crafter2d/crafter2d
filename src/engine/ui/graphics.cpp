
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
   glRecti(x, y, x+width, y+height);
}

void Graphics::drawRect(int x, int y, int width, int height)
{
   glRecti(x, y, x+1, y+height);                // left
	glRecti(x, y, x+width, y+1);                 // top
   glRecti(x+width-1, y, x+width, y+height);    // right
   glRecti(x, y+height-1, x+width, y+height);   // bottom
}

void Graphics::drawRoundedRect(int x, int y, int width, int height)
{
   glRecti(x, y+1, x+1, y+height-1);                // left
	glRecti(x+1, y, x+width-1, y+1);                 // top
   glRecti(x+width-1, y+1, x+width, y+height-1);    // right
   glRecti(x+1, y+height-1, x+width-1, y+height);   // bottom
}

void Graphics::drawText(int x, int y, const std::string& text)
{
   ASSERT_PTR(mpFont);

   glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT); 
   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

   float modelview_matrix[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

   glPushMatrix();
   glLoadIdentity();
   glTranslatef(x, y, 0);
   glMultMatrixf(modelview_matrix);

   float modelview_matrix2[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix2);

   mpFont->render(text);

   glPopMatrix();

   glPopAttrib();
}

void Graphics::translate(float x, float y)
{
   glMatrixMode(GL_MODELVIEW);
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
