/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "guitext.h"

#include <stdio.h>
#include <string.h>

#include "guifont.h"

void GuiText::printf (const GuiFont& font, int x, int y, const char* line, ...)
{
	char text[256];
	va_list ap;
	if (line == NULL)
		return;

	// format the input text
	va_start (ap, line);
	vsprintf (text, line, ap);
	va_end (ap);

	glRasterPos2i (x, y);

	font.draw(text);
}

void GuiText::printfn (const GuiFont& font, int x, int y, const char* line, int length)
{
	glRasterPos2i (x, y);

	font.draw(line, length);
}

void GuiText::printfn(const GuiFont& font, int x, int y, const std::string& str)
{
   printfn(font, x, y, str.c_str(), str.length());
}
