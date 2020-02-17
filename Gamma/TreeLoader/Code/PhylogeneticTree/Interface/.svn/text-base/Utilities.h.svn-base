#ifndef __PhylogeneticTree_Interface_Utilities_h__
#define __PhylogeneticTree_Interface_Utilities_h__

#include <ostream>

class RandomNumberGenerator;

namespace PhylogeneticTree
{
  class Tree;
  class TreeIterator;

  // BurnLeaf takes the index of a node to remove.  If the node is a leaf 
  //   it burns all node parents above it until it reaches a bifurcation.
  void BurnLeaf(Tree &, TreeIterator &);

  // Incomplete
  const bool CyclesFound(const Tree &);

  // Prints information about the tree onto the screen
  const bool PrintInformation(const Tree &, std::ostream &);

  // Removes nodes from the tree that are not part of the lineages formed
  //   by the leaf nodes in list of leaf nodes we care about.  Basically,
  //   it strips out all the leaf nodes we do not care about and their
  //   lineages that do not overlap with lineages we are keeping.
  const bool PruneDeadLeaves(Tree &);

  // Removes all nodes from the tree that has only a single child including
  //   the root.
  void RemoveNonfurcatingNodes(Tree &);

  // This function takes a tree and samples it's leaf nodes.  howManyLeaves 
  //   specifies how many leaf nodes, up to the total leaf nodes, that will 
  //   be sampled without replacement.  These new leaf nodes will then be 
  //   used to create a new tree that is returned.
  Tree *Sample(const Tree &,
               const unsigned int howManyLeaves,
               RandomNumberGenerator &) throw(int);

  // Incomplete
  const bool Validate(const Tree &);

} // namespace PhylogeneticTree

#endif // __PhylogeneticTree_Interface_Utilities_h__
