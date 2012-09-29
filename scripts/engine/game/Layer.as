
package engine.game;

use engine.core.*;

class Layer
{
	private Size            mSize 	   = null;
	private TileSet         mTileSet   = new TileSet();
	private TileMap         mMap       = null;
	private TileCoordinates mCoords    = null;
	
	// Get/set
	
	public Size getSize()
	{
		return mSize;
	}
	
	public TileMap getTileMap()
	{
		return mMap;
	}
	
	public TileCoordinates getTileCoordinates()
	{
		return mCoords;
	}
	
	// Designing
	
	public void prepare()
	{
		Effect effect = getEffect();
		Texture image = effect.getDiffuseTexture();
		
		string name = image.getName();
		int pos = name.indexOf('.');
		name = name.subString(0, pos + 1) + "xml";
		System.console.println("Using file: " + name);
		mTileSet.create(name);
		
		mCoords = new TileCoordinates();
		mCoords.initialize(image, mTileSet);
		
		fillMap();
	}
	
	private void fillMap()
	{
		int width = getWidth();
		int height = getHeight();
		
		mSize = new Size(width, height);
		mMap = new TileMap(mSize);
		
		for ( int y = 0; y < height; ++y )
		{
			for ( int x = 0; x < width; ++x )
			{
				int tile = getTile(x, y);
				mMap.setTile(x, y, tile);
			}
		}
	}
	
	// Natives
	
	public native Effect getEffect();
	
	private native int getWidth();
	private native int getHeight();
	private native int getTile(int x, int y);
	private native void setTile(int x, int y, int tile);
}
