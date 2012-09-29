
package engine.game;

use engine.io.*;
use engine.xml.*;

class TileSet
{
	private int mTileCount;
	private int mTileWidth;
	private int mTileHeight;
	
	public boolean create(string effect)
	{
		File file = FileSystem.getInstance().open(effect, 1);
		if ( file == null )
		{
			return false;
		}
		
		XmlDocument doc = new XmlDocument(file);
		XmlElement element = doc.getDocumentElement();
		mTileCount  = (int) element.getAttribute("count").getValue();
		mTileWidth  = (int) element.getAttribute("width").getValue();
		mTileHeight = (int) element.getAttribute("height").getValue();
		
		return true;
	}
	
	// Query
	
	public int getTileCount()
	{
		return mTileCount;
	}
	
	public int getTileWidth()
	{
		return mTileWidth;
	}
	
	public int getTileHeight()
	{
		return mTileHeight;
	}
}
