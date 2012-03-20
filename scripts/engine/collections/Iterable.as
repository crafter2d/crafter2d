
package engine.collections;

use iterator;

public abstract interface Iterable<T>
{
	public abstract Iterator<T> iterator();
}
