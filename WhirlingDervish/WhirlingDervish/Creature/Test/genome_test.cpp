#include <iostream>

#include "genome.h"
#include "instruction_set.h"

using namespace std;

void InstructionStatus(QString name, const Instruction &instruction);

int main(void) {
  cout << "Start genome test" << endl;

  cout << "End genome test" << endl;

  return 0;
}

void InstructionStatus(QString name, const Instruction &instruction) {
  cout << "Instruction(" << name << ")-" << endl;
  cout << "  name: " << instruction.Name() << endl;
  cout << "  valid: " << instruction.IsValid() << endl;
  cout << "  Genome: " << instruction.Stringify() << endl;
  cout << "End instruction" << endl;

  return;
}
