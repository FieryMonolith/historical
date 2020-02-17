#include <iostream>

#include "instruction_set.h"

using namespace std;

void InstructionStatus(QString name, const Instruction &instruction);

int main(void) {
  cout << "Start instruction test" << endl;

  Instruction *instruction, *instruction2;

  Split sSplit("split", 0, 10.0, -1.0, 0.0233, 0.33, 0.100, 24);
  InstructionStatus(QString("sSplit"), sSplit);
  Split *pSplit = new Split("split", 1, 10.0, -1.0, 0.0233, 0.33, 0.100, 24);
  InstructionStatus(QString("*pSplit"), *pSplit);
  instruction = new Split("split", 1, 10.0, -1.0, 0.0233, 0.33, 0.100, 24);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pSplit->Copy();
  InstructionStatus(QString("copy of pSplit"), *instruction2);
  delete instruction2;
  delete pSplit;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  SplitDup sSplitDup("split_dup", 0, 10.0, -1.0, 0.0233, 0.33, 0.100, 24);
  InstructionStatus(QString("sSplitDup"), sSplitDup);
  SplitDup *pSplitDup = new SplitDup("split_dup", 1, 10.0, -1.0, 0.0233, 
				     0.33, 0.100, 24);
  InstructionStatus(QString("*pSplitDup"), *pSplitDup);
  instruction = new SplitDup("split_dup", 1, 10.0, -1.0, 0.0233, 
			     0.33, 0.100, 24);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pSplitDup->Copy();
  InstructionStatus(QString("copy of pSplitDup"), *instruction2);
  delete instruction2;
  delete pSplitDup;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  Merge sMerge("merge", 10, 20);
  InstructionStatus(QString("sMerge"), sMerge);
  Merge *pMerge = new Merge("merge", 10, 20);
  InstructionStatus(QString("*pMerge"), *pMerge);
  instruction = new Merge("merge", 10, 20);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pMerge->Copy();
  InstructionStatus(QString("copy of pMerge"), *instruction2);
  delete instruction2;
  delete pMerge;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  AddLine sAddLine("add_line", 11, 20.0, 0.333, 1.251);
  InstructionStatus(QString("sAddLine"), sAddLine);
  AddLine *pAddLine = new AddLine("add_line", 11, 20.0, 0.333, 1.251);
  InstructionStatus(QString("*pAddLine"), *pAddLine);
  instruction = new AddLine("add_line", 11, 20.0, 0.333, 1.251);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pAddLine->Copy();
  InstructionStatus(QString("copy of pAddLine"), *instruction2);
  delete instruction2;
  delete pAddLine;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  RemoveLine sRemoveLine("remove_line", 12);
  InstructionStatus(QString("sRemoveLine"), sRemoveLine);
  RemoveLine *pRemoveLine = new RemoveLine("remove_line", 12);
  InstructionStatus(QString("*pRemoveLine"), *pRemoveLine);
  instruction = new RemoveLine("remove_line", 12);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pRemoveLine->Copy();
  InstructionStatus(QString("copy of pRemoveLine"), *instruction2);
  delete instruction2;
  delete pRemoveLine;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  ChangeMass sChangeMass("change_mass", 13.1);
  InstructionStatus(QString("sChangeMass"), sChangeMass);
  ChangeMass *pChangeMass = new ChangeMass("change_mass", 13.1);
  InstructionStatus(QString("*pChangeMass"), *pChangeMass);
  instruction = new ChangeMass("change_mass", 13.1);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pChangeMass->Copy();
  InstructionStatus(QString("copy of pChangeMass"), *instruction2);
  delete instruction2;
  delete pChangeMass;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  ChangeInnateLength sChangeInnateLength("change_innate_length", 14, 20.12);
  InstructionStatus(QString("sChangeInnateLength"), sChangeInnateLength);
  ChangeInnateLength *pChangeInnateLength = 
    new ChangeInnateLength("change_innate_length", 14, 20.12);
  InstructionStatus(QString("*pChangeInnateLength"), *pChangeInnateLength);
  instruction = new ChangeInnateLength("change_innate_length", 14, 20.12);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pChangeInnateLength->Copy();
  InstructionStatus(QString("copy of pChangeInnateLength"), *instruction2);
  delete instruction2;
  delete pChangeInnateLength;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  ChangeKS sChangeKS("change_ks", 15, 20.15);
  InstructionStatus(QString("sChangeKS"), sChangeKS);
  ChangeKS *pChangeKS = new ChangeKS("change_ks", 15, 20.15);
  InstructionStatus(QString("*pChangeKS"), *pChangeKS);
  instruction = new ChangeKS("change_ks", 15, 20.15);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pChangeKS->Copy();
  InstructionStatus(QString("copy of pChangeKS"), *instruction2);
  delete instruction2;
  delete pChangeKS;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  ChangeKD sChangeKD("change_kd", 16, 20.16);
  InstructionStatus(QString("sChangeKD"), sChangeKD);
  ChangeKD *pChangeKD = new ChangeKD("change_kd", 16, 20.16);
  InstructionStatus(QString("*pChangeKD"), *pChangeKD);
  instruction = new ChangeKD("change_kd", 16, 20.16);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pChangeKD->Copy();
  InstructionStatus(QString("copy of pChangeKD"), *instruction2);
  delete instruction2;
  delete pChangeKD;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  MoveTo sMoveTo("move_to", 17);
  InstructionStatus(QString("sMoveTo"), sMoveTo);
  MoveTo *pMoveTo = new MoveTo("move_to", 17);
  InstructionStatus(QString("*pMoveTo"), *pMoveTo);
  instruction = new MoveTo("move_to", 17);
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pMoveTo->Copy();
  InstructionStatus(QString("copy of pMoveTo"), *instruction2);
  delete instruction2;
  delete pMoveTo;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl;

  Nop sNop("nop");
  InstructionStatus(QString("sNop"), sNop);
  Nop *pNop = new Nop("nop");
  InstructionStatus(QString("*pNop"), *pNop);
  instruction = new Nop("nop");
  InstructionStatus(QString("instruction"), *instruction);

  instruction2 = pNop->Copy();
  InstructionStatus(QString("copy of pNop"), *instruction2);
  delete instruction2;
  delete pNop;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of instruction"), *instruction2);
  delete instruction2;
  delete instruction;
  instruction2 = instruction = NULL;

  cout << endl << endl;

  cout << "Test failures" << endl << endl;

  cout << "split command" << endl;
  cout << "Incorrect name (not split)" << endl;
  instruction = new Split("wrong", 1, -10.0, 10.0, 10.0, 1.0, 0.33, 12);
  InstructionStatus(QString("split"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new Split("split", -1, -10.0, 10.0, 10.0, 1.0, 0.33, 12);
  InstructionStatus(QString("split"), *instruction);
  delete instruction;
  cout << "Incorrect innate_length (<=0)" << endl;
  instruction = new Split("split", 1, -10.0, 10.0, 10.0, -1.0, 0.33, 12);
  InstructionStatus(QString("split"), *instruction);
  delete instruction;
  cout << "Incorrect ks (<=0)" << endl;
  instruction = new Split("split", 1, -10.0, 10.0, 10.0, 1.0, -0.33, 12);
  InstructionStatus(QString("split"), *instruction);
  delete instruction;
  cout << "Incorrect kd (<=0)" << endl;
  instruction = new Split("split", 1, -10.0, 10.0, 10.0, 1.0, 0.33, -12);
  InstructionStatus(QString("split"), *instruction);

  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "SplitDup command" << endl;
  cout << "Incorrect name (not SplitDup)" << endl;
  instruction = new SplitDup("wrong", 1, -10.0, 10.0, 10.0, 1.0, 0.33, 12);
  InstructionStatus(QString("SplitDup"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new SplitDup("SplitDup", -1, -10.0, 10.0, 10.0, 1.0, 0.33, 12);
  InstructionStatus(QString("SplitDup"), *instruction);
  delete instruction;
  cout << "Incorrect innate_length (<=0)" << endl;
  instruction = new SplitDup("SplitDup", 1, -10.0, 10.0, 10.0, -1.0, 0.33, 12);
  InstructionStatus(QString("SplitDup"), *instruction);
  delete instruction;
  cout << "Incorrect ks (<=0)" << endl;
  instruction = new SplitDup("SplitDup", 1, -10.0, 10.0, 10.0, 1.0, -0.33, 12);
  InstructionStatus(QString("SplitDup"), *instruction);
  delete instruction;
  cout << "Incorrect kd (<=0)" << endl;
  instruction = new SplitDup("SplitDup", 1, -10.0, 10.0, 10.0, 1.0, 0.33, -12);
  InstructionStatus(QString("SplitDup"), *instruction);

  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "merge command" << endl;
  cout << "Incorrect name (not merge)" << endl;
  instruction = new Merge("wrong", 2, 2);
  InstructionStatus(QString("merge"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new Merge("merge", -2, 2);
  InstructionStatus(QString("merge"), *instruction);
  delete instruction;
  cout << "Incorrect newid (<0)" << endl;
  instruction = new Merge("merge", 2, -2);
  InstructionStatus(QString("merge"), *instruction);

  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "AddLine command" << endl;
  cout << "Incorrect name (not AddLine)" << endl;
  instruction = new AddLine("wrong", 3, 1.0, 0.33, 12);
  InstructionStatus(QString("AddLine"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new AddLine("add_line", -3, 1.0, 0.33, 12);
  InstructionStatus(QString("AddLine"), *instruction);
  delete instruction;
  cout << "Incorrect innate_length (<=0)" << endl;
  instruction = new AddLine("add_line", 3, -1.0, 0.33, 12);
  InstructionStatus(QString("AddLine"), *instruction);
  delete instruction;
  cout << "Incorrect ks (<=0)" << endl;
  instruction = new AddLine("add_line", 3, 1.0, -0.33, 12);
  InstructionStatus(QString("addLine"), *instruction);
  delete instruction;
  cout << "Incorrect kd (<=0)" << endl;
  instruction = new AddLine("add_line", 3, 1.0, 0.33, -12);
  InstructionStatus(QString("AddLine"), *instruction);

  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "RemoveLine command" << endl;
  cout << "Incorrect name (not RemoveLine)" << endl;
  instruction = new RemoveLine("wrong", 4);
  InstructionStatus(QString("RemoveLine"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new RemoveLine("remove_line", -4);
  InstructionStatus(QString("RemoveLine"), *instruction);
  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "ChangeMass command" << endl;
  cout << "Incorrect name (not ChangeMass)" << endl;
  instruction = new ChangeMass("wrong", -0.333);
  InstructionStatus(QString("ChangeMass"), *instruction);
  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "ChangeInnateLength command" << endl;
  cout << "Incorrect name (not ChangeInnateLength)" << endl;
  instruction = new ChangeInnateLength("wrong", 5, -0.2);
  InstructionStatus(QString("ChangeInnateLength"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new ChangeInnateLength("change_innate_length", -5, 0.1);
  InstructionStatus(QString("ChangeInnateLength"), *instruction);
  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "ChangeKS command" << endl;
  cout << "Incorrect name (not ChangeKS)" << endl;
  instruction = new ChangeKS("wrong", 6, -0.2);
  InstructionStatus(QString("ChangeKS"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new ChangeKS("change_ks", -6, 0.1);
  InstructionStatus(QString("ChangeKS"), *instruction);
  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "ChangeKD command" << endl;
  cout << "Incorrect name (not ChangeKD)" << endl;
  instruction = new ChangeKD("wrong", 7, -0.2);
  InstructionStatus(QString("ChangeKD"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new ChangeKD("change_kd", -7, 0.1);
  InstructionStatus(QString("ChangeKD"), *instruction);
  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "MoveTo command" << endl;
  cout << "Incorrect name (not MoveTo)" << endl;
  instruction = new MoveTo("wrong", 8);
  InstructionStatus(QString("MoveTo"), *instruction);
  delete instruction;
  cout << "Incorrect id (<0)" << endl;
  instruction = new MoveTo("move_to", -8);
  InstructionStatus(QString("MoveTo"), *instruction);
  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "Nop command" << endl;
  cout << "Incorrect name (not Nop)" << endl;
  instruction = new Nop("wrong");
  InstructionStatus(QString("Nop"), *instruction);
  cout << "Copy invalid instruction" << endl;
  instruction2 = instruction->Copy();
  InstructionStatus(QString("copy of invalid instruction"), *instruction2);
  delete instruction;
  delete instruction2;

  cout << endl;

  cout << "End instruction test" << endl;

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
