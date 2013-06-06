
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
		//tr.addClass(TestFile.class);
		tr.addClass(TestClass.class);
		tr.addClass(TestInterface.class);
		tr.addClass(TestNative.class);
		tr.addClass(TestString.class);
		tr.addClass(TestChar.class);
		tr.addClass(TestArray.class);
		tr.addClass(TestMap.class);
		tr.addClass(TestLoops.class);
		tr.addClass(TestGenerics.class);
		tr.addClass(TestException.class);
		tr.addClass(TestSwitch.class);
		
		tr.run();
	}
}
