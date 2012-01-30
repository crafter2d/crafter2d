
// class used for generics testing

class GenericClass<E>
{
	private E mElement;
	
	public void set(E element)
	{
		mElement = element;
	}
	
	public E get()
	{
		return mElement;
	}
}
