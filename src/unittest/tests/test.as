
// c++ unittest test class

use System.*;
use UnitTest.*;

class Test
{
	public void run()
	{
		TestRunner tr = TestRunner.getInstance();
		tr.run(TestException.class);
		tr.run(TestLoops.class);
		tr.run(TestGenerics.class);
	}
}
