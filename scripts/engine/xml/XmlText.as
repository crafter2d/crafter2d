
package engine.xml;

class XmlText extends XmlNode
{
	// - Query
	public boolean isBlank()
	{
		return mValue.isEmpty();
	}
	
	// - Parsing
	
	protected void parse(XmlStream stream)
	{
		mValue = stream.readText('<', false);
	}
	
	// - Debuging
	
	protected void doPrint()
	{
		System.console.println(mValue);
	}
}
