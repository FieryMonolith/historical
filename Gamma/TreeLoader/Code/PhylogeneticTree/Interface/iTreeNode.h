#ifndef __PhylogeneticTree_Interface_iTreeNode_h__
#define __PhylogeneticTree_Interface_iTreeNode_h__

namespace PhylogeneticTree
{
  class iOrganism;

  class iTreeNode
  {
  protected:
    // The outside world should not be deleting iTreeNodes
    virtual ~iTreeNode(void) { return; }

  public:
    virtual const iOrganism   &GetData(void)         const = 0;
    virtual const unsigned int HowManyChildren(void) const = 0;
  };

} // namespace PhylogeneticTree

#endif // __PhylogeneticTree_Interface_iTreeNode_h__
