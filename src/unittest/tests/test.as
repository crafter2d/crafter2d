
// c++ unittest test class
// Called from the UnitTest application to verify the behavior of the script language
// and standard classes that are delivered with it.

use System.*;
use UnitTest.*;

class Test
{
	public void run()
	{
		TestRunner tr = TestRunner.getInstance();
		tr.addClass(TestMap.class);
		tr.addClass(TestException.class);
		tr.addClass(TestLoops.class);
		tr.addClass(TestGenerics.class);
		tr.run();
	}
}
