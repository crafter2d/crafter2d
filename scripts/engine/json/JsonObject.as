
package engine.json;

use engine.collections.Map;

class JsonObject extends Json
{
	private Map<string, Json> mChildren;
	
	public void add(string name, Json value)
	{
		mChildren.insert(name, value);
	}
}
