
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

class String;
class UIFont;
class Texture;

class Graphics
{
public:
   Graphics();

   void fillRect(int x, int y, int width, int height);

   void drawRect(int x, int y, int width, int height);
   void drawRoundedRect(int x, int y, int width, int height);
	void drawText(int x, int y, const String& text);
   void drawTexture(const Texture& texture, int x, int y, int width, int height);
   void drawTexture(const Texture& texture, int x, int y, int width, int height, float tx, float ty, float tw, float th);

   void translate(float x, float y);

   void setFont(UIFont& font);
   void setColor(float r, float g, float b, float a);

private:
   UIFont* mpFont;
};

#endif // ENGINE_GRAPHICS_H_
