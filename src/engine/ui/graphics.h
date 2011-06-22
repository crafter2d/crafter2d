
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>

class Font;

class Graphics
{
public:
   Graphics();

   void fillRect(int x, int y, int width, int height);

   void drawRect(int x, int y, int width, int height);
   void drawRoundedRect(int x, int y, int width, int height);
	void drawText(int x, int y, const std::string& text);
	
   void translate(float x, float y);

   void setFont(Font& font);
   void setColor(float r, float g, float b, float a);

private:
   Font* mpFont;
};

#endif // ENGINE_GRAPHICS_H_
