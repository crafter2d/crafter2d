
package engine.ui;

use engine.core.*;
use engine.shapes.Rectangle;

class Image extends Widget
{
	private Texture   mTexture;
	private boolean   mStretch;
	private Rectangle mRect;
	
	public Image(string name)
	{
		super();
		
		mTexture = TextureManager.getInstance().getTexture(name);
		mRect = new Rectangle();
	}
	
	// - Size
	
	public Size getPreferredSize()
	{
		return mTexture.getSize();
	}
	
	protected void boundsChanged()
	{
		Rectangle bounds = getBounds();
		Size texsize = mTexture.getSize();
		int x = (bounds.width / 2) - (texsize.width / 2);
		int y = (bounds.height / 2) - (texsize.height / 2);
		mRect.set(x, y, texsize.width, texsize.height);
	}
	
	// - Painting
	
	protected void paintBackground(Graphics graphics)
	{
		
	}
	
	protected void paintWidget(Graphics graphics)
	{
		graphics.drawTexture(mTexture, mRect);
	}
}
