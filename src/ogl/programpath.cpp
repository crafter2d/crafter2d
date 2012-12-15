
#include "programpath.h"

using namespace Graphics;

ProgramPath::ProgramPath():
   CodePath(),
   program()
{
}

ProgramPath::~ProgramPath()
{
}

bool ProgramPath::load (const String& vertex, const String& fragment)
{
	// forget about the fragment shader, it is not supported
	if ( !program.compile(vertex) )
		return false;

	return true;
}

void ProgramPath::release ()
{
	program.release ();
}

void ProgramPath::enable () const
{
	program.enable();
}

void ProgramPath::disable () const
{
	program.disable();
}

CodePath::PathType ProgramPath::getType() const
{
   return CodePath::EGLSL;
}

int ProgramPath::getUniformLocation (const String& name) const
{
	return program.getUniformLocation(name);
}

void ProgramPath::setStateUniform(int index)
{
}

void ProgramPath::setUniform1i (int index, int val)
{
}

void ProgramPath::setUniform3f (int index, float x, float y, float z)
{
}

void ProgramPath::setUniform4f (int index, float x, float y, float z, float w)
{
    glProgramLocalParameter4fARB (GL_VERTEX_PROGRAM_ARB, index, x, y, z, w);
}
