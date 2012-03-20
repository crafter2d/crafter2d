
package engine.collections;

class MapPair<K, E>
{
	private K mKey;
	private E mElement;
	
	public MapPair(K key, E element)
	{
		super();
		mKey = key;
		mElement = element;
	}
	
	public K getKey() 
	{
		return mKey;
	}
	
	public E getElement()
	{
		return mElement;
	}
}
