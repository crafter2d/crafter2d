
package engine.ui;

use engine.ui.*;
use engine.shapes.*;

class MessageBox extends Dialog
{
	public static void showInformation(string message)
	{
		showInformation(message, "Information");
	}
	
	public static void showInformation(string message, string title)
	{
		MessageBox msgbox = new MessageBox(title);
		msgbox.create(message, title);
		msgbox.show();
	}

	private MessageBox(string title)
	{
		super(title);
	}
	
	private void create(string message, string title)
	{
		Button button = new Button("Ok");
		button.setName("btnName");
		
		Image image = new Image("cancel.png");
		image.setName("imgInfo");
		
		Label label = new Label();
		label.setName("lblMsg");
		label.setText(message);
		
		setName("dlgMessage");
		setBounds(new Rectangle(5, 5, 200, 100));
		setLayout(new BorderLayout());
		add(button, BorderLayoutSide.bottom);
		add(image, BorderLayoutSide.left);
		add(label, BorderLayoutSide.center);
		validate();
	}
	
	private void show()
	{
		Canvas canvas = Canvas.getInstance();
		canvas.add(this);
	}
}
