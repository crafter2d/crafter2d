
package engine.xml;

class XmlDeclaration extends XmlNode
{
	private string mVersion;
	private string mEncoding;
	private string mStandalone;
	
	// - Parsing
	
	protected void parse(XmlStream stream)
	{
		XmlAttribute attr = new XmlAttribute();
		
		while ( !stream.isEOS() )
		{
			stream.skipWhitespace();
			if ( stream.peek() == '>' )
			{
				break;
			}

			if ( stream.peek("version") )
			{
				attr.parse(stream);
				mVersion = attr.getValue();
			}
			else if ( stream.peek("encoding") )
			{
				attr.parse(stream);
				mEncoding = attr.getValue();
			}
			else if ( stream.peek("standalone") )
			{
				attr.parse(stream);
				mStandalone = attr.getValue();
			}
			else
			{
				while ( !stream.isEOS() )
				{
					char next = stream.peek();
					if ( next == '>' || Char.isWhitespace(next) )
					{
						break;
					}
					
					stream.skip();
				}
			}			
		}
		
		stream.skip(); // read the end >
	}
	
	// - Debugging
	
	protected void doPrint()
	{
		System.console.println("<?xml");
		System.console.println(">");
	}
}
