
package engine.xml;

class XmlText extends XmlNode
{
	private string mText;
	
	// - Query
	public boolean isBlank()
	{
		return mText.isEmpty();
	}
	
	// - Parsing
	
	protected void parse(XmlStream stream)
	{
		mText = stream.readText('<', false);
		
		System.console.println("Text: " + mText);

		char[] endtag = { '<', '/' };
		if ( stream.peek(endtag) ) 
		{
			System.console.println("end tag detected");
			// hit a end tag
			return;
		}
		else
		{
			// hit new element
		}
	}
}
