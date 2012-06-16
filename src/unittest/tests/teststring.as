
// c++ unit testing string

use system.*;

class TestString
{
	@test
	public void testStringCompare()
	{
		string one = "one";
		string two = "one";
		string three = "three";
		
		assert one == two;
		assert one.equals(two);
		assert one != three;
		assert !one.equals(three);
	}
	
	@test
	public void testStringLength()
	{
		string str = "a string";
		assert str.length() == 8; 
	}
	
	@test
	public void testSubstring()
	{
		string str = "abcdefgh";
		
		string sub1 = str.subString(0, 3);
		assert sub1 == "abc";
		
		string sub2 = str.subString(4, 4);
		assert sub2 == "efgh";
	}
}
