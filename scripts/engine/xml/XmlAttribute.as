
package engine.xml;

class XmlAttribute extends XmlNode
{
	private string mName;
	private string mValue;
	
	// - Get/set
	
	public string getName()
	{
		return mName;
	}
	
	public string getValue()
	{
		return mValue;
	}
	
	// - Parsing
	
	protected void parse(XmlStream stream)
	{
		mName = stream.readName();
		
		stream.skipWhitespace();
		
		char assign = stream.get();
		assert assign == '=';
		
		stream.skipWhitespace();
		
		char next = stream.get();
		if ( next == '\'' )
		{
			mValue = stream.readText('\'', false);
			stream.skip();
		}
		else if  ( next == '\"' )
		{
			mValue = stream.readText('\"', false);
			stream.skip();
		}
		else
		{
			mValue = mValue + next;
			while ( !stream.isEOS() )
			{
				next = stream.peek();
				if ( Char.isWhitespace(next) || next == '/' || next == '>' )
				{
					// end of the value
					break;
				}
				else if ( next == '\'' || next == '\"' )
				{
					throw new XmlError("Error reading attribute value: unexpected (double) quote");
				}
				
				mValue = mValue + next;
				stream.skip();
			}
		}
	}
	
	// - Debugging
	
	protected void doPrint()
	{
		System.console.println(mName + "= \"" + mValue + "\" ");
	}
}
