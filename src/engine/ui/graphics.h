
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>

class Graphics
{
public:
   Graphics();

   void fillRect(int x, int y, int width, int height);
	void drawText(int x, int y, const std::string& text);
	void translate(float x, float y);
   void setColor(float r, float g, float b, float a);
};

#endif // ENGINE_GRAPHICS_H_
