
package engine.xml;

class XmlDeclaration extends XmlNode
{
	private string mVersion;
	private string mEncoding;
	private string mStandalone;
	
	// - Parsing
	
	protected void parse(XmlStream stream)
	{
		while ( !stream.isEOS() )
		{
			stream.skipWhitespace();
			if ( stream.peek() == '>' )
			{
				break;
			}

			if ( stream.peek("version") )
			{
				XmlAttribute attr = new XmlAttribute();
				attr.parse(stream);
				mVersion = attr.getValue();
			}
			else if ( stream.peek("encoding") )
			{
				XmlAttribute attr = new XmlAttribute();
				attr.parse(stream);
				mEncoding = attr.getValue();
			}
			else if ( stream.peek("standalone") )
			{
				XmlAttribute attr = new XmlAttribute();
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
		
		stream.get(); // read the end >
	}
}
