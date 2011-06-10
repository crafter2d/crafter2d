
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
	
	public void add(T element)
	{
		mArray[mElements++] = element;
	}
	
	public void remove(T element)
	{
		int index = indexOf(element);
		if ( index < mElements )
		{
			// copy all from index+1 to index
		}
	}
	
	public int size()
	{
		return mElements;
	}
	
	public int indexOf(T element)
	{
		for ( int index = 0; index < mElements; index++ )
		{
			if ( mArray[index] == element )
			{
				return index;
			}
		}
		return mElements;
	}
	
	public Iterator<T> iterator()
	{
		return new ArrayIterator<T>(mArray, mElements);
	}
}
