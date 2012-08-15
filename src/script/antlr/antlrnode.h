
#ifndef ANTLR_NODE_H_
#define ANTLR_NODE_H_

#include "core/string/string.h"

struct ANTLR3_BASE_TREE_struct;

class AntlrNode
{
public:
   AntlrNode(ANTLR3_BASE_TREE_struct* mpNode);

 // query
   bool isNil() const;

   int       getChildCount() const;
   AntlrNode getChild(int index) const;

   int getLine() const;
   int getPosition() const;

   String toString() const;
   int getType() const;

private:
   ANTLR3_BASE_TREE_struct* mpNode;
};

#endif // ANTLR_NODE_H_
