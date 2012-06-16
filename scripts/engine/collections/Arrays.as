
package engine.collections;

final class Arrays
{
	static public <Type> void copy(Type[] from, Type[] to)
	{
		assert from.length <= to.length;
		for ( int index = 0; index < from.length; index++ )
		{
			to[index] = from[index];
		}
	}
	
	static public <Type> boolean equals(Type[] left, Type[] right)
	{
		return equals(left, right, left.length);
	}
	
	static public <Type> boolean equals(Type[] left, Type[] right, int length)
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
