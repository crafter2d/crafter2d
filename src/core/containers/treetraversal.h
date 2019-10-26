#ifndef TREE_TRAVERSAL_H
#define TREE_TRAVERSAL_H

#include <deque>

template <class E>
class ITreeTraversal
{
public:
   virtual const TreeNode<E>* node() const = 0;
   virtual TreeNode<E>* node() = 0;

   virtual void traverse() = 0;

   virtual E& element() = 0;
};

template <class E>
class TreeDepthFirstTraversal : public ITreeTraversal<E>
{
   class StackNode
   {
   public:
      explicit StackNode(TreeNode<E>& node) : mNode(node), child(0) {}

      const TreeNode<E>& node() const { return mNode; }
      TreeNode<E>& node() { return mNode; }

      TreeNode<E>& childNode() {
         assert(child < mNode.getChildren().size());
         return mNode[child];
      }

      bool done() const {
         return child >= mNode.getChildren().size();
      }

      TreeNode<E>& mNode;
      int child;
   };

public:
   TreeDepthFirstTraversal(TreeNode<E>& node) : mStack() { mStack.emplace_back(node); }

   virtual const TreeNode<E>* node() const override {
      return mStack.empty() ? nullptr : &mStack.back().node();
   }

   virtual TreeNode<E>* node() override {
      return mStack.empty() ? nullptr : &mStack.back().node();
   }

   virtual void traverse() override {
      if ( !mStack.empty() )
      {
         StackNode& stacknode = mStack.back();
         if ( !stacknode.done() )
         {
            mStack.emplace_back(stacknode.childNode());
            stacknode.child++;
         }
         else
         {
            do {
               mStack.pop_back();
            } while ( !mStack.empty() && mStack.back().done() );
         }
      }
   }

   virtual E& element() override {
      if ( mStack.empty() )
         throw std::runtime_error("Invalid iterator");
      return *mStack.back().node();
   }
  
private:

 // members
   std::deque<StackNode> mStack;
};

#endif // TREE_TRAVERSAL_H
