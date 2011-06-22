
package System;

class NativeFunctionNotFoundException extends Throwable
{
	public void setCause(string cause)
	{
		super.setCause("Native function '" + cause + "' not found.");
	}
}
