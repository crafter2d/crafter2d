
// c++ unit testing char

use system.*;

class TestChar
{
	@test
	public void testWhitespace()
	{
		char iswhitespace = ' ';
		char nonwhitespace = 'a';
		
		assert Char.isWhitespace(' ');
		assert Char.isWhitespace(iswhitespace);
		assert !Char.isWhitespace(nonwhitespace);
	}
	
	@test
	public void testAlphaDigitDetection()
	{
		char alpha = 'a';
		char digit = '1';
	
		assert Char.isAlpha(alpha);
		assert !Char.isAlpha(digit);
		assert Char.isDigit(digit);
		assert !Char.isDigit(alpha);
		assert Char.isAlphaNum(alpha);
		assert Char.isAlphaNum(digit);
		assert !Char.isAlphaNum('#');
	}
	
	@test
	public void testUnicodeEncoding()
	{
		char unicode = '\u0066';
		
		assert unicode == 'f';
		assert unicode == '\u0066';
	}
}
