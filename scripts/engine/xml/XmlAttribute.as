
package engine.xml;

class XmlAttribute extends XmlNode
{
	private string mName;
	private string mValue;
	
	// - Parsing
	
	protected void parse(XmlStream stream)
	{
		mName = stream.readName();
		
		char assign = stream.get();
		assert assign == '=';
		
		char next = stream.get();
		if ( next == '\'' )
		{
			mValue = stream.readText('\'', false);
			stream.get();
		}
		else if  ( next == '\"' )
		{
			mValue = stream.readText('\"', false);
			stream.get();
		}
		else
		{
			mValue = mValue + next;
			while ( !stream.isEOS() )
			{
				next = stream.get();
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
			}
		}
		
		System.console.println("Attribute: " + mName + ": "+ mValue);
	}
}
