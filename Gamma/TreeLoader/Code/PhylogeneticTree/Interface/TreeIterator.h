#ifndef __PhylogeneticTree_Interface_TreeIterator_h__
#define __PhylogeneticTree_Interface_TreeIterator_h__

namespace PhylogeneticTree
{
  class iTreeNode;
  class TreeNode;

  // The TreeIterator uses a specific ordering for tree traversal.
  //   It first assumes that any given node can have more than two
  //   children and that there are no cycles.  For a simplified
  //   description consider a tree with a root and three children
  //   the order that will be retrieved will be
  //   child1, child2, child3, root

  class TreeIterator
  {
  private:
    TreeNode *node;

  public:
    TreeIterator(TreeNode *startNode=0);
    TreeIterator(const TreeIterator &);
    ~TreeIterator(void);

    const TreeIterator &operator=(const TreeIterator &);

    // Traversal
    void operator++(void); // prefix
    void operator++(int);  // postfix
    void operator--(void); // prefix
    void operator--(int);  // postfix
    void Up(void);

    // Data Access
    const iTreeNode *operator*(void) const; // dereference

  private:
    void Next_Depth(void);
    void Previous_Depth(void);

    friend const bool operator< (const TreeIterator &, const TreeIterator &);
    friend const bool operator==(const TreeIterator &, const TreeIterator &);
    friend const bool operator!=(const TreeIterator &, const TreeIterator &);

    friend class Tree;
  };

  const bool operator< (const TreeIterator &, const TreeIterator &);
  const bool operator==(const TreeIterator &, const TreeIterator &);
  const bool operator!=(const TreeIterator &, const TreeIterator &);

} // namespace PhylogeneticTree

#endif // __PhylogeneticTree_Interface_TreeIterator_h__