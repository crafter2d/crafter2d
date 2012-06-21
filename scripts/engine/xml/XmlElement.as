
package engine.xml;

class XmlElement extends XmlNode
{
	private string mValue;
	
	protected void parse(XmlStream stream)
	{
		char entry = stream.get();
		assert entry == '<';
		
		mValue = stream.readName();
		if ( mValue.length() == 0 )
		{
			throw new XmlError("Element without name found");
		}
		
		System.console.println("Element: " + mValue);
		
		stream.skipWhitespace();
		
		while ( !stream.isEOS() )
		{
			char next = stream.get();
			
			if ( next == '/' )
			{
				// empty tag
			}
			else if ( next == '>' )
			{
				// done with attributes, now read the value
				parseValue(stream);
				
				string endtag = "</" + mValue;
				if ( !stream.read(endtag) )
				{
					throw new XmlError("Missing end tag");
				}
				
				char c = stream.get();
				if ( c != '>' )
				{
					throw new XmlError("Error reading end tag");
				}
				
				System.console.println("end of element " + mValue);
				
				break;
			}
			else
			{
				System.console.println("Reading attribute...");
				
				// try to read an attribute
				XmlAttribute attr = new XmlAttribute();
				attr.parse(stream);
				
				addChild(attr);
			}
		}
	}
	
	private void parseValue(XmlStream stream)
	{
		while ( !stream.isEOS() )
		{
			char next = stream.peek(0);
			if ( next == '<' )
			{
				char[] endtag = { '<', '/' };
				if ( stream.peek(endtag) )
				{
					return;
				}
				
				// another element
				XmlNode node = identify(stream);
				node.parse(stream);
				addChild(node);
			}
			else
			{
				XmlText text = new XmlText();
				text.parse(stream);
			
				if ( !text.isBlank() )
				{
					addChild(text);
				}
			}
		}
		
		System.console.println("end of parseValue");
	}
}
	