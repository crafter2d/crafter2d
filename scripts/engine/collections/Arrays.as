
package engine.collections;

final class Arrays
{
	static public <Type> void copy(Type[] from, Type[] to)
	{
		assert from.size() <= to.size();
		for ( int index = 0; index < from.size(); index++ )
		{
			to[index] = from[index];
		}
	}
	
	static public <Type> boolean equals(Type[] left, Type[] right)
	{
		return equals(left, right, left.size());
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
