
package system;

/// This class should not be used directly. It is part of the virtual machine.

final class InternalString
{
	public char[] asArray()
	{
		char[] buffer = new char[length()];
		for ( int index = 0; index < length(); index++ )
		{
			buffer[index] = getChar(index);
		}
		return buffer;
	}
	
	public boolean isEmpty()
	{
		return length() == 0;
	}
	
	/// compares the strings, returns true when equal
	public native boolean equals(string s);

	/// returns the length of the string
	public native int length();
	
	/// returns the character at the given index
	public native char getChar(int index);
	
	/// returns a sub string of this string starting at character start
	/// and a length of count
	public native string subString(int start, int count);
	
	/// returns the index of the first occurence of character c in this string
	public native int indexOf(char c);
	
	/// returns the index of the last occurence of character c in this string
	public native int lastIndexOf(char c);
}
