
// c++ unittest test class

use System.*;
use UnitTest.*;

class Test
{	
	public void run()
	{
		Class cl = engine.collections.ArrayList.class;
		TestRunner tr = TestRunner.getInstance();
		
		tr.run(TestException.class);
		tr.run(TestLoops.class);
	}
}
 