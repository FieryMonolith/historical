#ifndef __PhylogeneticTree_Include_TreeNode_h__
#define __PhylogeneticTree_Include_TreeNode_h__

#include <vector>

#include "PhylogeneticTree/Interface/iTreeNode.h"

namespace PhylogeneticTree
{
  class iOrganism;

  class TreeNode : public iTreeNode
  {
  private:
    iOrganism &data;
    TreeNode *parent;
    std::vector<TreeNode*> children;
    bool deleteMe;

  public:
    TreeNode(const iOrganism &);
    ~TreeNode(void);

    // Available through iTreeNode
    const iOrganism   &GetData(void)         const;
    const unsigned int HowManyChildren(void) const;

    // Hidden through iTreeNode as this header is not intended to be 
    //   available outside this package.
    void                          ChangeChild(const TreeNode &from,
                                              const TreeNode &to);
    const std::vector<TreeNode*> &GetChildren(void)             const;
    const bool                    GetDeleteMe(void)             const;
    TreeNode*                     GetParent(void)               const;
    void                          InsertChild(const TreeNode &) throw(int);
    void                          RemoveChild(const TreeNode &);
    void                          SetAsRoot(void);
    void                          SetDeleteMe(const bool);
    void                          SetParent(const TreeNode &);

  private:
    TreeNode(const TreeNode &);
    const TreeNode &operator=(const TreeNode &);
  };

} // namespace PhylogeneticTree

#endif // __PhylogeneticTree_Include_TreeNode_h__
