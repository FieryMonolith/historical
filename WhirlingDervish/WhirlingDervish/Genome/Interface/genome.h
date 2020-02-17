#ifndef __GENOME_H__
#define __GENOME_H__

#include <vector>
#include <ostream>

class Instruction;

class Genome {
public:
  Genome(void);
  ~Genome(void);

  bool Copy(const Genome &);
  bool CopyInstructions(const std::vector<Instruction *> &);
  void Clear(void);

  bool Stringify(std::ostream &) const;
  // Going to assume people won't be changing the instructions
  const std::vector<Instruction *> &Instructions(void) const;

  int operator==(const Genome &) const;
  int operator!=(const Genome &) const;

  //int Type(void) const { return d_type; }
  //void SetType(int type) { d_type = type; return; }

  //int Hash(void) const { return d_hash; }

  int &Mutations(void) { return d_mutations; }

private:
  int d_mutations;
  //mutable int d_hash;
  std::vector<Instruction *> d_instructions;

  int IsEqual(const Genome &) const;
  //void CalculateHash(void) const;
};

#endif
