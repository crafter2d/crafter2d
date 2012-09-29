
package engine.xml;

use engine.io.*;

class XmlDocument extends XmlNode
{	
	private XmlStream 	mStream;
	private string 		mFilename;
	
	public XmlDocument(File file)
	{
		super();
		
		mStream = new XmlStream(file);
		
		parse(mStream);
	}
	
	// - Query
	
	public XmlElement getDocumentElement()
	{
		foreach ( XmlNode node : mChildren )
		{
			if ( node instanceof XmlElement )
			{
				return (XmlElement) node;
			}
		}
		return null;
	}
	
	// - Parsing
	
	protected void parse(XmlStream stream)
	{
		while ( !stream.isEOS() )
		{
			XmlNode node = identify(stream);
			node.parse(stream);
			
			stream.skipWhitespace();
		}
	}
	
	// - Debugging
	
	protected void doPrint()
	{
		printChildren();
	}
}
