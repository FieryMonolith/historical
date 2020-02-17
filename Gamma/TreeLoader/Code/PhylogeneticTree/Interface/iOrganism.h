#ifndef __PhylogeneticTree_Interface_OrganismInterface_h__
#define __PhylogeneticTree_Interface_OrganismInterface_h__

namespace PhylogeneticTree
{

  class iOrganism
  {
  public:
    virtual ~iOrganism(void) { return; }

    virtual const int    GetId(void)        const = 0;
    virtual const int    GetParentId(void)  const = 0;
    virtual const double GetBirthTime(void) const = 0;
    virtual const bool   GetIsAlive(void)   const = 0;
  };

} // namespace PhylogeneticTree

#endif // __PhylogeneticTree_Interface_OrganismInterface_h__
