
package engine.ui;

use engine.shapes.*;

class DialogBorder extends Border
{
	private Dialog mDialog;
	
	public DialogBorder(Dialog dialog)
	{
		super();
		
		mDialog = dialog;
		
		getMargins().set(2, 15, 2, 2);
	}
	
	public void paint(Graphics graphics)
	{
		Rectangle rect = mDialog.getBounds();
		
		graphics.setColor(Color.red);
		
		// left side
		Rectangle borderrect = new Rectangle(0, 0, 2, rect.height);
		graphics.fillRect(borderrect);
		
		// top
		borderrect.set(0, 0, rect.width, 15);
		graphics.fillRect(borderrect);
		
		// right
		borderrect.set(rect.width - 2, 0, 2, rect.height);
		graphics.fillRect(borderrect);
		
		// bottom
		borderrect.set(0, rect.height - 2, rect.width, 2);
		graphics.fillRect(borderrect);
	}
}
