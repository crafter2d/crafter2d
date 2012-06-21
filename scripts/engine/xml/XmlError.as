
package engine.xml;

class XmlError extends Throwable
{
	public XmlError()
	{
		super();
	}
	
	public XmlError(string error)
	{
		super(error);
	}
}
