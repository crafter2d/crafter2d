
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
	
	protected void parse(XmlStream stream)
	{
		XmlNode node = identify(stream);
		node.parse(stream);
	}
}
