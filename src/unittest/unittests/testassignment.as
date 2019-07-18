
// c++ unit testing map

use engine.collections.*;
use system.*;

class TestAssignment
{
	private static int VALUE = 15;
	private int mValue = 0;
	
	@test
	public void testLocalVariable()
	{
		int value = 5;
		assert value == 5;
	}
	
	@test
	public void testMemberVariable()
	{
		assert mValue == 0;
		mValue = 14;
		assert mValue == 14;
		mValue = TestAssignment.VALUE;
		assert mValue == 15;
	}
}
