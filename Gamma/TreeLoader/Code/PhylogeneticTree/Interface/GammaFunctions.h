#ifndef __PhylogeneticTree_Interface_GammaFunctions_h__
#define __PhylogeneticTree_Interface_GammaFunctions_h__

class OutputStream;

namespace PhylogeneticTree
{
  class Tree;

  double ComputeGamma(const Tree &,
                      const double timeCutoff,
                      const char *const outFilename,
                      OutputStream &output) throw(int);

} // namespace PhylogeneticTree

#endif // __PhylogeneticTree_Interface_GammaFunctions_h__
