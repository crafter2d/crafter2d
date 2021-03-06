
package engine.xml;

use engine.collections.*;

class XmlElement extends XmlNode
{	
	private ArrayList<XmlAttribute> mAttributes = new ArrayList<XmlAttribute>();
	
	// Query
	
	public XmlAttribute getAttribute(string name)
	{
		foreach ( XmlAttribute attr : mAttributes )
		{
			if ( attr.getName() == name )
			{
				return attr;
			}
		}
		return null;
	}
	
	// Parsing
	
	protected void parse(XmlStream stream)
	{		
		mValue = stream.readName();
		if ( mValue.length() == 0 )
		{
			throw new XmlError("Element without name found");
		}
		
		stream.skipWhitespace();
		
		while ( !stream.isEOS() )
		{
			char next = stream.peek();
			
			if ( next == '/' )
			{
				// empty tag
				stream.skip();
				stream.skip(); // skip also closing >
				
				break;
			}
			else if ( next == '>' )
			{
				stream.get();
				
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
				break;
			}
			else
			{
				// try to read an attribute
				XmlAttribute attr = new XmlAttribute();
				attr.parse(stream);
				
				mAttributes.add(attr);
			}
			
			stream.skipWhitespace();
		}
	}
	
	private void parseValue(XmlStream stream)
	{
		while ( !stream.isEOS() )
		{
			char next = stream.peek();
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
	}
	
	protected void doPrint()
	{
		System.console.println("<" + mValue + " ");
		foreach ( XmlAttribute attr : mAttributes )
		{
			attr.prettyPrint();
		}
		
		System.console.println(">");
		
		printChildren();
	}
}
	