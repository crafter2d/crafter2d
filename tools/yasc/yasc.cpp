// compiler.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include "core/commandline/commandline.h"

#include "compiler/compiler.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
   Compiler compiler(argc, argv);
   return compiler.exec();
}

