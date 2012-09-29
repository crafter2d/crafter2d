
package engine.game;

use engine.core.*;

class TileMap
{
	private Size  mSize;
	private int[] mMap;
	
	public TileMap(Size size)
	{
		super();
		
		mSize = size;
		mMap = new int[size.width * size.height];
	}
	
	public Size getSize()
	{
		return mSize;
	}
	
	public void setTile(int x, int y, int tile)
	{
		mMap[y * mSize.width + x] = tile;
	}
	
	public int getTile(int x, int y)
	{
		return mMap[y * mSize.width + x];
	}
}
