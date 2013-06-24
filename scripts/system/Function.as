
package system;

public class Function
{
	private string mName;
	private string[] mAnnotations;
	
	public string getName()
	{
		return mName;
	}
	
	public boolean hasAnnotation(string name)
	{
		for ( int index = 0; index < mAnnotations.size(); index++ )
		{
			if ( mAnnotations[index] == name )
			{
				return true;
			}
		}
		return false;
	}
	
	public void invoke(Object object)
	{
		doInvoke(object);
	}
	
	private native void doInvoke(Object object);
}
