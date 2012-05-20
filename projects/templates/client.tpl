
package {{PACKAGE}};

use engine.ui.*;
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
			MessageBox.showInformation("Hello world!");
			
			return true;
		}
		
		return false;
	}
}