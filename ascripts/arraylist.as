
use iterable;
use arrayiterator;
use arrayindexoutofboundsexception;

class ArrayList<T> implements Iterable<T>
{
	private Object[] mArray;
	private int      mElements;
	
	public ArrayList()
	{
		super();
		
		mArray = new Object[100];
		mElements = 0;
	}
	
	public T get(int index)
	{
		if ( index >= mArray.length )
		{
			throw new ArrayIndexOutOfBoundsException();
		}
		return (T) mArray[index];
	}
	
	public void add(T t)
	{
		mArray[mElements++] = t;
	}
	
	public int size()
	{
		return mElements;
	}
	
	public Iterator<T> iterator()
	{
		return new ArrayIterator<T>(mArray, mElements);
	}
}
