
#ifndef ANTLR_NODE_H_
#define ANTLR_NODE_H_

#include "core/string/string.h"

struct ANTLR3_BASE_TREE_struct;

class AntlrTextPosition;

class AntlrNode
{
public:
   AntlrNode(ANTLR3_BASE_TREE_struct* mpNode);

 // query
   bool isNil() const;

   int       getChildCount() const;
   AntlrNode getChild(int index) const;

   AntlrTextPosition getPosition() const;

   String toString() const;
   int getType() const;

private:

 // get/set
   int getLine() const;
   int getPos() const;

 // data
   ANTLR3_BASE_TREE_struct* mpNode;
};

#endif // ANTLR_NODE_H_
