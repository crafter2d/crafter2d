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
#ifndef GUIMANAGER_H_
#define GUIMANAGER_H_

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "core/math/point.h"
#include "core/math/color.h"

#include "guidefines.h"
#include "guihelpers.h"

class GuiFont;
class GuiWnd;
class GuiDialog;
class GuiMenu;
class GuiPopupDecorator;
class TiXmlElement;

class GuiManager
{
public:
	                     ~GuiManager();
	static GuiManager&   getInstance();

	bool                 initialize();
   void                 destroy();

   FT_Library           getFreeTypeLib();

   void                 showPopup(GuiWnd& owner, const Point& point, const std::string& name);

   bool                 isDialog(const std::string& name);
   bool                 isMenu(const std::string& name);

   GuiWnd*              loadFromXML(const std::string& name);
   GuiDialog*           loadDialogFromXML(const std::string& name);
   GuiMenu*             loadMenuFromXML(const std::string& name);
   GuiDialog*           loadSystemDialog(const std::string& name);

	void                 setDefaultFont(GuiFont* f);
   GuiFont*             getDefaultFont() const;

   void                 setDefaultTextColor(const Color& color);
   const Color&         getDefaultTextColor() const;

protected:
	                     GuiManager();

private:
   GuiDialog*  loadDialogFromXML(TiXmlElement& dialog);
   GuiMenu*    loadMenuFromXML(TiXmlElement& menu);

   std::string constructPath(const std::string& filename);

	FT_Library           mFreeTypeLib;
   Color                mTextColor;
   GuiFont*             mpFont;
   GuiPopupDecorator*   mpPopup;
};

#ifdef JENGINE_INLINE
#  include "guimanager.inl"
#endif

#endif
