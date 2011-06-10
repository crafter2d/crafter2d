
package engine.ui;

use engine.shapes.*;

abstract class Graphics
{	
	abstract public void fillRect(Rectangle rect);
	
	abstract public void drawText(int x, int y, string text);
	
	abstract public void translate(int x, int y);
	
	abstract public Color getColor();
	abstract public void setColor(Color color);
}
