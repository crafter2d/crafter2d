
package engine.io;

final class FileSystem
{
	public static native FileSystem getInstance();
	
	public File open(string name, int modus)
	{
		File file = native;
		return file;
	}
}
