
use unittest.testrunner;

use arraylist;
use unit;
use world;

class Army
{
	ArrayList<Unit> mUnits = new ArrayList<Unit>();
	
	public static void executeTests()
	{	
		World world = new World();
		System.console.println("World: " + world.getName());
		
		if ( ClassLoader.getInstance().isLoaded("Army") )
		{
			System.console.println("Loaded army");
		}
		else
		{
			System.console.println("Army is not loaded!!??");
		}
		
		TestRunner runner = TestRunner.getInstance();
		runner.run(Army.class);
	}
	
	public Army()
	{
	}
	
	@pretest
	public void fill()
	{
		mUnits.add(new Unit("joop"));
		mUnits.add(new Unit("kees"));
		mUnits.add(new Unit("mies"));
	}
	
	@test
	public string testBitwiseOperator()
	{
		int FLAG_ONE = 1;
		int FLAG_TWO = 2;
		
		string result = "";
		int flags = FLAG_ONE | FLAG_TWO;
		if ( (flags & FLAG_ONE) == FLAG_ONE )
		{
			result = "correct";
		}
		else
		{
			result = "incorrect";
		}
		return result;
	}
	
	@test
	public string testForeach()
	{
		string unitname = "";
		foreach( Unit u : mUnits )
		{
			unitname = unitname + u.getName() + " ";
		}
		return unitname;
	}
	
	@test
	public void testIterator()
	{
		string result = "";

		Iterator<Unit> it = mUnits.iterator();
		while ( it.hasNext() )
		{
			result = result + it.next().getName();
			if ( it.hasNext() )
			{
				result = result + " ";
			}
		}

		System.console.println(result);
	}
		
	@test
	public string testContinue()
	{
		string result = "";
		for ( int index = 0; index < mUnits.size(); index++ )
		{
			if ( index == 1 )
			{
				continue;
			}
			result = result + " " + mUnits.get(index).mName;
		}
		return result;
	}
	
	@test
	public string testExceptions()
	{
		string result = "before";
		try
		{
			Unit invalid = mUnits.get(5);
		}
		catch ( Throwable e )
		{
			result = result + " catch";
		}
		finally
		{
			result = result + " finally";
		}
		return result;	
	}
}
