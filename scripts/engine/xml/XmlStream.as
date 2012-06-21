
package engine.xml;

use engine.io.*;

class XmlStream
{
	private int    mPos;
	private char[] mBuffer;
	
	public XmlStream(File file)
	{
		super();
		
		int length = file.length();
		if ( length <= 0 )
		{
			throw new XmlError("Empty document");
		}
		
		mBuffer = new char[length];
		file.read(mBuffer);
		mPos = 0;
	}
	
	// - Query
	
	public boolean isEOS()
	{
		return mPos >= mBuffer.length;
	}
	
	// - Operations
	
	public void skipWhitespace()
	{
		while ( mPos < mBuffer.length && Char.isWhitespace(mBuffer[mPos]) )
		{
			++mPos;
		}
	}
	
	public char peek(int dist)
	{
		return mBuffer[mPos + dist];
	}
	
	public char get()
	{		
		return mBuffer[mPos++];
	}
	
	public boolean peek(char[] sequence)
	{
		assert mBuffer != null;
		
		if ( mPos + sequence.length >= mBuffer.length )
		{
			// does not fit in the buffer
			return false;
		}

		for ( int index = 0; index < sequence.length; index++ )
		{
			if ( mBuffer[mPos + index] != sequence[index] )
			{
				return false;
			}
		}
		
		return true;
	}
	
	public boolean read(char[] sequence)
	{
		boolean result = peek(sequence);
		if ( result )
		{
			mPos = mPos + sequence.length;
		}
		
		return result;
	}
	
	public boolean read(string sequence)
	{
		char[] seq = sequence.asArray();
		return read(seq);
	}
	
	public string readName()
	{
		skipWhitespace();

		string result = "";
		if ( Char.isAlpha(mBuffer[mPos]) )
		{
			while ( mPos < mBuffer.length )
			{
				char c = mBuffer[mPos];
				if ( Char.isAlphaNum(c) || c == '_' || c == '-' || c == '.' || c == ':' )
				{
					result = result + c;
				}
				else
				{
					break;
				}
			
				++mPos;
			}
		}
		
		return result;
	}
	
	public string readText(char end, boolean skipwhitespace)
	{
		string result = "";
		
		while ( mPos < mBuffer.length )
		{
			char c = mBuffer[mPos];
			if ( c == end )
			{
				break;
			}
			
			result = result + c;
			
			++mPos;
		}
		
		return result;
	}
}
