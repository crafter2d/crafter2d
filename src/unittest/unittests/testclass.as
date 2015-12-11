
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
		// currently fails, needs to be implemented
		// DerivedClass.mStaticMember = 123;
	}
}
