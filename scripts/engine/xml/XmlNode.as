
package engine.xml;

use engine.collections.*;

abstract class XmlNode
{
	private static char[] xmlHeader = { '?', 'x', 'm', 'l' };
	private static char[] commentHeader = { '!', '-', '-' };
	private static char[] dtdHeader = { '!' };
	private static char[] cdataHeader = { '!', '[', 'C', 'D', 'A', 'T', 'A', '[' };
	
	private XmlNode 			mParent = null;
	private ArrayList<XmlNode> 	mChildren = new ArrayList<XmlNode>();
	protected string 			mValue;
	
	// - Get/set
	
	public XmlNode getParent()
	{
		return mParent;
	}
	
	public void setParent(XmlNode parent)
	{
		mParent = parent;
	}
	
	public string getValue()
	{
		return mValue;
	}
	
	public void addChild(XmlNode child)
	{
		mChildren.add(child);
	}
	
	// - Parsing
	
	abstract protected void parse(XmlStream stream);
	
	protected XmlNode identify(XmlStream stream)
	{
		XmlNode result = null;
		
		stream.skipWhitespace();
		
		char entry = stream.get();
		assert entry == '<';
		
		if (  stream.read(xmlHeader) )
		{
			result = new XmlDeclaration();
		}
		else if ( stream.read(commentHeader) )
		{
			result = new XmlComment();
		}
		else if ( stream.read(cdataHeader) )
		{
		}
		else if ( stream.read(dtdHeader) )
		{
		}
		else
		{
			char next = stream.peek();
			if ( Char.isAlpha(next) || next == '_' )
			{
				result = new XmlElement();
			}
		}
		
		if ( result == null )
		{
			throw new XmlError("Unexpected element found in XML file");
		}
		
		result.setParent(this);
		addChild(result);
		
		return result;
	}
}
