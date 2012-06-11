
package system;

/// This class should not be used directly. It is part of the virtual machine.

final class InternalString
{
	public char[] asArray()
	{
		char[] buffer = new char[length()];
		for ( int index = 0; index < length(); index++ )
		{
			buffer[index] = get(index);
		}
		return buffer;
	}
	
	/// compares the strings, returns true when equal
	public native boolean equals(string s);

	/// returns the length of the string
	public native int length();
	
	/// returns the character at the given index
	public native char get(int index);
	
	/// returns a sub string of this string starting at character start
	/// and a length of count
	public native string subString(int start, int count);
}
