
package engine.io;

final class FileSystem
{
	public static native FileSystem getInstance();
	
	public File open(string name, int modus)
	{
		File file = native_open(name, modus);
		return file;
	}
	
	private native File native_open(string name, int modus);
}
