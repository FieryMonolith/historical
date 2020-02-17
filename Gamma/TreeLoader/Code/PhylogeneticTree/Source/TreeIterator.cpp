#include "PhylogeneticTree/Interface/TreeIterator.h"

#include "PhylogeneticTree/Include/TreeNode.h"

using namespace std;

PhylogeneticTree::TreeIterator::TreeIterator(TreeNode *n) : node(n)
{
  return;
}

PhylogeneticTree::TreeIterator::TreeIterator(const TreeIterator &t)
: node(t.node)
{
  return;
}

PhylogeneticTree::TreeIterator::~TreeIterator(void)
{
  node = 0;
  return;
}

void PhylogeneticTree::TreeIterator::Next_Depth(void)
{
  if(node == 0) { return; }

  TreeNode *currentNode = node;

  // 1. Go up one node
  node = node->GetParent();

  // 2. If up is 0, we were at the root which is the last node.  It is now
  //      equivalent to the "end", ie. node == 0.
  if(node == 0) { return; }

  // 3. If the node we just left was last in the list of children for this
  //      this node, stop.
  if(node->GetChildren().back() == currentNode)
  {
    return;
  }

  // 4. Else goto next child in the list and all the way down to the left
  const vector<TreeNode*> &children = node->GetChildren();
  vector<TreeNode*>::const_iterator i = children.end();
  for(i = children.begin(); i != children.end(); ++i)
  {
    if(*i == currentNode)
    {
      ++i;
      node = *i;
      break;
    }
  }
  if(i == children.end() || node == 0)
  {
    node = 0;
    return;
  }

  // Traverse down the tree as far to the left as possible from node.  I 
  //   assume there are no cycles in the tree.
  while(node->HowManyChildren() > 0)
  {
    node = node->GetChildren().front();
  }

  return;
}

void PhylogeneticTree::TreeIterator::operator--(void)
{
  Previous_Depth();
  return;
}

void PhylogeneticTree::TreeIterator::operator--(int)
{
  Previous_Depth();
  return;
}

const PhylogeneticTree::iTreeNode *
PhylogeneticTree::TreeIterator::operator*(void) const
{
  return node;
}

const bool PhylogeneticTree::operator< (const TreeIterator &t1,
                                        const TreeIterator &t2)
{
  return t1.node < t2.node;
}

const bool PhylogeneticTree::operator==(const TreeIterator &t1,
                                        const TreeIterator &t2)
{
  return t1.node == t2.node;
}

const bool PhylogeneticTree::operator!=(const TreeIterator &t1,
                                        const TreeIterator &t2)
{
  return t1.node != t2.node;
}

void PhylogeneticTree::TreeIterator::operator++(void)
{
  Next_Depth();
  return;
}

void PhylogeneticTree::TreeIterator::operator++(int)
{
  Next_Depth();
  return;
}

const PhylogeneticTree::TreeIterator &
PhylogeneticTree::TreeIterator::operator=(const TreeIterator &t)
{
  node = t.node;
  return *this;
}

void PhylogeneticTree::TreeIterator::Previous_Depth(void)
{
  if(node == 0) { return; }

  // 1. If the node is not a leaf, go to last child
  if(node->HowManyChildren() > 0)
  {
    node = node->GetChildren().back();
    return;
  }

  TreeNode *currentNode = 0;

  do
  {
    currentNode = node;

    // 2. Go up one node
    node = node->GetParent();

    // 3. If up is 0, we were at the root which is the last node.  It is now
    //      equivalent to the "end", ie. node == 0.
    if(node == 0) { return; }

    // 3. If the node we just left was first in the list of children for this
    //      node go to step 1.
  }
  while(node->GetChildren().front() == currentNode);

  // 4. Once we reach a parent where the previous node was not first in the 
  //      list of children.  Find the previous child and go to that node.
  const vector<TreeNode*> &children = node->GetChildren();
  vector<TreeNode*>::const_iterator i = children.end();
  for(i = children.begin(); i != children.end(); ++i)
  {
    if(*i == currentNode)
    {
      --i;
      node = *i;
      break;
    }
  }
  if(i == children.end() || node == 0)
  {
    node = 0;
    return;
  }

  return;
}

void PhylogeneticTree::TreeIterator::Up(void)
{
  if(node == 0) { return; }
  node = node->GetParent();
  return;
}
