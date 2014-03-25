
package {{PACKAGE}};

use engine.game.*;

class {{PROJ}}Client extends Client
{
	public {{PROJ}}Client()
	{
		super();
	}
	
	public boolean create()
	{
		if ( super.create() )
		{	
			return true;
		}
		
		return false;
	}
}