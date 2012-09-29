
package engine.ui.tileeditor;

use engine.core.*;
use engine.game.*;
use engine.shapes.*;

class TileEditor
{
	private World mWorld;
	private Layer mLayer;
	
	// - get/set
	
	public void setWorld(World world)
	{
		mWorld = world;
		if ( mWorld != null )
		{
			mWorld.prepare();
		}
	}
	
	// - painting
	
	protected void paintWidget(Graphics graphics)
	{
		if ( mWorld != null )
		{
			ArrayList<Layer> layers = mWorld.getLayers();
			foreach ( Layer layer : layers )
			{
				paintLayer(graphics, layer);
			}
		}
	}
	
	private void paintLayer(Graphics graphics, Layer layer)
	{
		Effect effect = layer.getEffect();
		Texture image = effect.getDiffuseTexture();
		int imagewidth = image.getWidth();
		int imageheight = image.getHeight();
		
		Size size = layer.getSize();
		TileMap tilemap = layer.getTileMap();
		TileCoordinates coords = layer.getTileCoordinates();
		
		Rectangle pos = new Rectangle();
		Rectangle tex = new Rectangle();
		
		real texwidth = coords.getTileWidth();
		real texheight =
		
		for ( int y = 0; y < size.height; ++y )
		{
			for ( int x = 0; x < size.width; ++x )
			{
				int tex = tilemap.getTile(x, y);
				if ( tex != -1 )
				{
					pos.set(x * imagewidth, y * imageheight, imagewidth, imageheight);
					Vector2D texcoord = coords.getCoordinate(tex);
					graphics.drawTexture(image, pos, texcoord.x, texcoord.y, texwidth, texheight);
				}
			}
		}
	}
}
