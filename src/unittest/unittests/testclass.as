
// c++ unit testing for classes

class TestClass
{
	@test
	public void testCallBaseFunction()
	{
		DerivedClass klass = new DerivedClass();
		klass.setMemberBase(123);
		assert klass.getMemberBase() == 123;
	}
	
	@test void testDerivedClassAlterBaseMember()
	{
		DerivedClass klass = new DerivedClass();
		klass.setMemberDerived(456);
		assert klass.getMemberBase() == 456;
	}
	
	@test
	void testUseStaticOfCorrectClass()
	{
		BaseClass.mStaticMember = 123;
		assert BaseClass.mStaticMember == 123;
	}
	
	@test
	void testUseStaticOfBaseClass()
	{
		DerivedClass.mStaticMember = 125;
		assert BaseClass.mStaticMember == 125;
	}
	
	@test
	void testUseProtectedStaticOfBaseClass()
	{
		DerivedClass klass = new DerivedClass();
		int value = klass.getStaticProtectedMember();
		assert value == 1;
	}
	
	@test
	void testCallSuperFunction()
	{
		DerivedClass klass = new DerivedClass();
		string value = klass.getBaseValue();
		assert value == "value";
	}
}
