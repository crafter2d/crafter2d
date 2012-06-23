
package engine.xml;

class XmlComment extends XmlNode
{
	private string mValue;
	
	// - Parsing
	protected void parse(XmlStream stream)
	{
		char[] endtag = { '-', '-', '>' };
		
		// ignore elements within the comments
		while  ( !stream.isEOS() && !stream.read(endtag) )
		{
			mValue = mValue + stream.get();
		}
		
		System.console.println("Comment: " + mValue);
	}
}
