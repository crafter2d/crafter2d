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
#include "guiwnd.h"

#include "guilist.h"
#ifndef JENGINE_INLINE
#  include "guilist.inl"
#endif

GuiList::GuiNode::GuiNode(GuiWnd* w, GuiNode* n, GuiNode* p):
   wnd(w),
   next(n),
   prev(p)
{
   if (n != 0)
   {
      n->prev = this;
   }
}

GuiList::Iterator::Iterator():
   cur(0)
 {
 }

GuiList::Iterator::Iterator(GuiNode *n):
   cur(n)
{
}

GuiList::GuiList():
   head(0),
   size(0)
{
}

GuiList::~GuiList()
{
   removeAll();
}

void GuiList::add(GuiWnd* wnd)
{
	GuiNode* n = new GuiNode (wnd, head);
	head = n;
   size++;
}

void GuiList::addTail(GuiWnd* wnd)
{
   GuiNode* n = 0;
   if (head == 0) {
      n = new GuiNode(wnd, 0, 0);
      head = n;
   }
   else {
      GuiNode* temp = head;
      while (temp->next) temp = temp->next;
      n = new GuiNode(wnd, 0, temp);
      temp->next = n;
   }
}

void GuiList::insert(const Iterator& it, GuiWnd* wnd, bool before)
{
   if ( !it.valid() )
   {
      addTail(wnd);
   }
   else
   {
      GuiNode* n = new GuiNode(wnd, NULL, NULL);
      if ( it.cur->prev == NULL && before )
      {
         n->next = head;
         head = n;
      }
      else
      {
         if ( before )
         {
            n->next = it.cur;
            n->prev = it.cur->prev;
   
            it.cur->prev->next = n;
            it.cur->prev = n;
         }
         else
         {
            n->next = it.cur->next;
            n->prev = it.cur;
   
            it.cur->next->prev = n;
            it.cur->next = n;
         }
      }
   }
}

void GuiList::removeHead()
{
   if (head) {
      // remove the head
      GuiNode* temp = head;
      head = head->next;
      if (head) head->prev = NULL;
   
      //delete temp->wnd;
      delete temp;
      size--;
   }
}

void GuiList::removeAt(Iterator& it)
{
   if ( it.valid() )
   {
      // inlink the window from the list
      if (it.cur->prev)
         it.cur->prev->next = it.cur->next;
      if (it.cur->next)
         it.cur->next->prev = it.cur->prev;

      // make sure we have a valid head pointer
      if (it.cur == head)
         head = head->next;

      GuiNode* temp = it.cur;
      it.cur = it.cur->next;
      delete temp;
   }
}

void GuiList::removeAll()
{
   GuiNode* pcur = head;
   while ( pcur != NULL )
   {
      GuiNode* ptemp = pcur;
      pcur = pcur->next;
      delete ptemp;
   }
   
   head = NULL;
}

GuiList::Iterator GuiList::find(GuiWnd* wnd)
{
   GuiNode* node = head;
   while (node) {
      if (node->wnd == wnd)
         return Iterator(node);
      node = node->next;
   }
   return Iterator();
}

GuiList::Iterator GuiList::end() const
{
   if (!head) return Iterator();
   GuiNode* temp = head;
   while (temp->next) temp = temp->next;
   return Iterator(temp);
}
