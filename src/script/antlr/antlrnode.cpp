
#include "antlrnode.h"

#include "script/output/asParser.h"

AntlrNode::AntlrNode(ANTLR3_BASE_TREE_struct* pnode):
   mpNode(pnode)
{
}

// query

bool AntlrNode::isNil() const
{
   return mpNode == NULL || mpNode->isNilNode(mpNode);
}
   
int AntlrNode::getChildCount() const
{
   return mpNode->getChildCount(mpNode);
}
   
AntlrNode AntlrNode::getChild(int index) const
{
   return AntlrNode((ANTLR3_BASE_TREE_struct*)mpNode->getChild(mpNode, index));
}

int AntlrNode::getType() const
{
   return mpNode->getType(mpNode);
}

String AntlrNode::toString() const
{
   return String((char*)mpNode->toString(mpNode)->chars);
}

int AntlrNode::getLine() const
{
   return mpNode->getLine(mpNode);
}

int AntlrNode::getPosition() const
{
   return mpNode->getCharPositionInLine(mpNode);
}
