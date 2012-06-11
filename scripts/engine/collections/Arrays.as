
package engine.collections;

final class Arrays
{
	public boolean compareN(char[] left, char[] right, int length)
	{
		for ( int index = 0; index < length; index++ )
		{
			if ( left[index] != right[index] )
			{
				return false;
			}
		}
		return true;
	}
}
