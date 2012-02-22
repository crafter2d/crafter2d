
#ifndef ANTLR_NODE_H_
#define ANTLR_NODE_H_

#include <string>

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

   std::string toString() const;
   int getType() const;

private:
   ANTLR3_BASE_TREE_struct* mpNode;
};

#endif // ANTLR_NODE_H_
