
package engine.ui;

use engine.shapes.*;
use engine.core.*;

abstract class Graphics
{	
	abstract public void fillRect(Rectangle rect);
	
	abstract public void drawText(int x, int y, string text);
	abstract public void drawRect(Rectangle rect);
	abstract public void drawRoundedRect(Rectangle rect);
	
	abstract public void drawTexture(Texture texture, Rectangle bounds);
	abstract public void drawTexture(Texture texture, Rectangle bounds, real tx, real ty, real tw, real th);
	
	abstract public void translate(int x, int y);
	
	abstract public Color getColor();
	abstract public void setColor(Color color);
	abstract public void setFont(Font font);
}
