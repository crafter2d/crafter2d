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
#ifndef GUILIST_H_
#define GUILIST_H_

class GuiWnd;

class GuiList
{
	struct GuiNode {
      GuiNode(GuiWnd* w, GuiNode* n, GuiNode* p=0);
		GuiWnd* wnd;
		GuiNode* next;
		GuiNode* prev;
	};
	
public:
   class Iterator {
	public:
		         Iterator();

      void     operator++();
      void     operator--();

		GuiWnd*  operator* ();

      bool     operator!=(const Iterator& it) const;
      bool     valid() const;
		
	protected:
		friend class GuiList;
               Iterator(GuiNode *n);
		GuiNode* cur;
	};

	            GuiList();
	            ~GuiList();
	
	void        add (GuiWnd* wnd);
   void        addTail(GuiWnd* wnd);
   void        insert(const Iterator& it, GuiWnd* wnd, bool before = true);

   void        removeHead();
   void        removeAt(Iterator& it);
   void        removeAll();

   int         count() const;
   bool        empty() const;

   Iterator    find(GuiWnd* wnd);
   Iterator    begin() const;
   Iterator    end() const;
	
private:
   int      size;
	GuiNode  *head;
};

#ifdef JENGINE_INLINE
#  include "guilist.inl"
#endif

#endif
