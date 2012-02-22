
package engine.collections;

class Map<K, E>
{
	private ArrayList< MapPair<K, E> > mPairs = new ArrayList< MapPair<K, E> >();
	
	public void insert(K key, E element)
	{
		MapPair<K, E> pair = new MapPair<K, E>(key, element);
		mPairs.add(pair);
	}
	
	public E get(K key)
	{
		foreach ( MapPair<K, E> pair : mPairs )
		{
			if ( pair.getKey() == key )
			{
				return pair.getElement();
			}
		}
		return null;
	}
}
