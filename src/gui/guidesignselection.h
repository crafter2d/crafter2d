//
// C++ Interface: guidesignselection
//
// Description: 
//
//
// Author: Jeroen Broekhuizen <jeroen@jengine.homedns.org>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUIDESIGNSELECTION_H
#define GUIDESIGNSELECTION_H

#include "guicontrol.h"

class GuiDesignWnd;

/**
@author Jeroen Broekhuizen
*/
class GuiDesignSelection : public GuiControl
{
public:
   enum Sides { ELeft=1, ERight=2, ETop=4, EBottom=8, ENone=0 };

   GuiDesignSelection();
   virtual ~GuiDesignSelection();

 // Operations
   void  resize(const GuiPoint& rel, int borders);
   int   determineBorder(const GuiPoint& point) const;

 // Input interface
   virtual int    onLButtonDown(const GuiPoint& point, int flags);
   virtual int    onLButtonUp(const GuiPoint& point, int flag);
   virtual void   onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flag);
    
 // Overloads
   virtual void      onRender(Uint32 tick, const GuiGraphics& graphics);
   virtual GuiWnd*   hitTest(const GuiPoint& point);

private:
   GuiDesignWnd*  getDesignWnd() const;

   int _border;
};

#endif
