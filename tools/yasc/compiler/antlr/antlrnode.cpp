
#include "antlrnode.h"

#include "yasc/compiler/output/asParser.h"

#include "antlrtextposition.h"

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
   return AntlrNode(static_cast<ANTLR3_BASE_TREE_struct*>(mpNode->getChild(mpNode, index)));
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

int AntlrNode::getPos() const
{
   return mpNode->getCharPositionInLine(mpNode);
}

AntlrTextPosition AntlrNode::getPosition() const
{
   return AntlrTextPosition(getLine(), getPos());
}

