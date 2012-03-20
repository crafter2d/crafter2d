
package engine.collections;

use Iterator;

class ArrayIterator<T> extends Iterator<T>
{
	private Object[] mArray;
	private int      mSize;
	private int      mIndex;
	
	public ArrayIterator(Object[] array, int size)
	{
		super();
		
		mArray = array;
		mSize  = size;
		mIndex = 0;
	}
	
	public boolean hasNext()
	{
		return mIndex < mSize;
	}
	
	public T next()
	{
		return (T) mArray[mIndex++];
	}
}
