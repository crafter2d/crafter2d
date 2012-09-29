
package engine.game;

use engine.core.*;

class TileCoordinates
{
	Vector2D[] mCoords = null;
	
	// tile size in texture space
	real     mTileWidth;
	real     mTileHeight;
	
	public TileCoordinates()
	{
		super();
	}
	
	// Get/set
	
	public Vector2D getCoordinate(int tile)
	{
		return mCoords[tile];
	}
	
	public real getTileWidth()
	{
		return mTileWidth;
	}
	
	public real getTileHeight()
	{
		return mTileHeight;
	}
	
	// Initialization
	
	public void initialize(Texture tex, TileSet info)
	{
		real border = 0.0;
		real maxTilesOnRow = (real) tex.getWidth() / (real) info.getTileWidth();
		System.console.println("Max tiles: " + maxTilesOnRow);
		
		real dx = tex.getSourceWidth() / tex.getWidth();
		real dy = tex.getSourceHeight() / tex.getHeight();
		
		mTileWidth = dx * info.getTileWidth();
		mTileHeight = dy * info.getTileHeight();
		
		real borderx = dx * (info.getTileWidth() + border * 2.0);
		real bordery = dy * (info.getTileHeight() + border * 2.0);
		
		real offsetx = (borderx - mTileWidth) / 2.0;
		real offsety = (bordery - mTileHeight) / 2.0;
		
		int count = info.getTileCount();
		mCoords = new Vector2D[count + 1];
		mCoords[0] = new Vector2D();
		for ( int index = 0; index < count; ++index )
		{
			mCoords[index + 1] = new Vector2D();
			mCoords[index+1].setX(offsetx + (index % maxTilesOnRow) * borderx);
			mCoords[index+1].setY(offsety + (index / maxTilesOnRow) * bordery);
		}
	}
}
