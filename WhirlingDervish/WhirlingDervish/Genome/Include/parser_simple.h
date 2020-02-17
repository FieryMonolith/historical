#ifndef __PARSER_SIMPLE_H__
#define __PARSER_SIMPLE_H__

#include <string>
#include <vector>

#include "boost/spirit/core.hpp"
#include "instruction_set_simple.h"

#include "parser.h"

using namespace boost::spirit;

class SimpleParser;

class Process {
public:
  Process(SimpleParser &p, SimpleParser::SimpleState &s, int w)
    : d_p(p), d_s(s), d_w(w) { return; }
  ~Process(void) { return; }

  template <typename IteratorT>
  void operator()(IteratorT, IteratorT) const {
    // Potential error due to unprotected tsp pointer in grammar
    d_s.d_instruction = d_w;
    d_p.CreateInstruction(d_s);
    return;
  }
  template <typename ValType> void operator()(ValType val) const {
    d_p.Update(d_s, d_w, val);
    return;
  }

private:
  SimpleParser &d_p;
  SimpleParser::SimpleState &d_s;
  int d_w;
};

class SimpleGrammar : public grammar<SimpleGrammar> {
public:
  SimpleParser &d_p;
  SimpleParser::SimpleState &d_s;

  SimpleGrammar(SimpleParser &p, SimpleParser::SimpleState &s)
    : d_p(p), d_s(s) { return; }
  ~SimpleGrammar(void) { return; }

  template <typename ScannerT> class definition {
  public:
    rule<ScannerT> split, split_dup, merge, remove_point, add_line;
    rule<ScannerT> remove_line, change_mass, change_fluctuation_delta;
    rule<ScannerT> change_fluctuation_start, change_ks, change_kd;
    rule<ScannerT> move_to, nop;

    rule<ScannerT> splitName, splitDupName, mergeName, removePointName;
    rule<ScannerT> removeLineName, changeMassName, changeFluctuationDeltaName;
    rule<ScannerT> changeFluctuationStartName, changeKSName, changeKDName;
    rule<ScannerT> moveToName, nopName, addLineName;

    rule<ScannerT> genome;
    rule<ScannerT> x, y, z, mass, ks, kd, fluctuation_delta, fluctuation_start;
    rule<ScannerT> position, spring_properties, id, id2, end_line;

    const rule<ScannerT> &start(void) const { return genome; }

    definition(const SimpleGrammar &self) {
      end_line = ch_p(';');

      x = real_p[Process(self.d_p, self.d_s, SimpleInstruction::X)];
      y = real_p[Process(self.d_p, self.d_s, SimpleInstruction::Y)];
      z = real_p[Process(self.d_p, self.d_s, SimpleInstruction::Z)];

      ks = real_p[Process(self.d_p, self.d_s, SimpleInstruction::KS)];
      kd = real_p[Process(self.d_p, self.d_s, SimpleInstruction::KD)];
      fluctuation_delta = real_p[Process(self.d_p, self.d_s,
					 SimpleInstruction::FDELTA)];
      fluctuation_start = real_p[Process(self.d_p, self.d_s,
					 SimpleInstruction::FSTART)];

      id = uint_p[Process(self.d_p, self.d_s, SimpleInstruction::ID1)];
      id2 = uint_p[Process(self.d_p, self.d_s, SimpleInstruction::ID2)];;
      mass = real_p[Process(self.d_p, self.d_s, SimpleInstruction::MASS)];;

      position = x >> ',' >> y >> ',' >> z;
      spring_properties = ks >> ',' >> kd >> ',' >> 
	fluctuation_delta >> ',' >> fluctuation_start;

      splitName = strlit<std::string::const_iterator>(
				    SimpleInstruction::splitName.begin(),
				    SimpleInstruction::splitName.end());
      splitDupName = strlit<std::string::const_iterator>(
				    SimpleInstruction::splitDupName.begin(),
				    SimpleInstruction::splitDupName.end());
      mergeName = strlit<std::string::const_iterator>(
				    SimpleInstruction::mergeName.begin(),
				    SimpleInstruction::mergeName.end());
      removePointName = strlit<std::string::const_iterator>(
				    SimpleInstruction::removePointName.begin(),
				    SimpleInstruction::removePointName.end());
      addLineName = strlit<std::string::const_iterator>(
				    SimpleInstruction::addLineName.begin(),
				    SimpleInstruction::addLineName.end());
      removeLineName = strlit<std::string::const_iterator>(
				    SimpleInstruction::removeLineName.begin(),
				    SimpleInstruction::removeLineName.end());
      changeMassName = strlit<std::string::const_iterator>(
				    SimpleInstruction::changeMassName.begin(),
				    SimpleInstruction::changeMassName.end());
      changeFluctuationDeltaName = strlit<std::string::const_iterator>(
			 SimpleInstruction::changeFluctuationDeltaName.begin(),
			 SimpleInstruction::changeFluctuationDeltaName.end());
      changeFluctuationStartName = strlit<std::string::const_iterator>(
			 SimpleInstruction::changeFluctuationStartName.begin(),
			 SimpleInstruction::changeFluctuationStartName.end());
      changeKSName = strlit<std::string::const_iterator>(
				    SimpleInstruction::changeKSName.begin(),
				    SimpleInstruction::changeKSName.end());
      changeKDName = strlit<std::string::const_iterator>(
				    SimpleInstruction::changeKDName.begin(),
				    SimpleInstruction::changeKDName.end());
      moveToName = strlit<std::string::const_iterator>(
				    SimpleInstruction::moveToName.begin(),
				    SimpleInstruction::moveToName.end());
      nopName = strlit<std::string::const_iterator>(
				    SimpleInstruction::nopName.begin(),
				    SimpleInstruction::nopName.end());

      split = splitName >> '(' >> id >> ',' >> mass  >> ',' >>
	position >> ',' >> spring_properties >> ')' >> end_line;

      split_dup = splitDupName >> '(' >> id >> ',' >> mass >> ',' >>
	position >> ',' >> spring_properties >> ')' >> end_line;

      merge = mergeName >> '(' >> id >> ',' >> id2 >> ')' >> end_line;

      remove_point = removePointName >> '(' >> id >> ')' >> end_line;

      add_line = addLineName >> '(' >> id >> ',' >> 
	spring_properties >> ')' >> end_line;

      remove_line = removeLineName >> '(' >> id >> ')' >> end_line;

      change_mass = changeMassName >> '(' >> mass >> ')' >> end_line;

      change_fluctuation_delta = changeFluctuationDeltaName >> '(' >>
	id >> ',' >> fluctuation_delta >> ')' >> end_line;

      change_fluctuation_start = changeFluctuationStartName >> '(' >>
	id >> ',' >> fluctuation_start >> ')' >> end_line;

      change_ks = changeKSName >> '(' >> id >> ',' >> ks >> ')' >> 
	end_line;

      change_kd = changeKDName >> '(' >> id >> ',' >> kd >> ')' >> 
	end_line;

      move_to = moveToName >> '(' >> id >> ')' >> end_line;

      nop = nopName >> '(' >> ')' >> end_line;

      genome = *(
	 split[Process(self.d_p, self.d_s, SimpleInstruction::SPLIT)]               |
	 split_dup[Process(self.d_p, self.d_s, SimpleInstruction::SPLIT_DUP)]       |
	 merge[Process(self.d_p, self.d_s, SimpleInstruction::MERGE)]               |
	 remove_point[Process(self.d_p, self.d_s, SimpleInstruction::REMOVE_POINT)] |
	 add_line[Process(self.d_p, self.d_s, SimpleInstruction::ADD_LINE)]         |
	 remove_line[Process(self.d_p, self.d_s, SimpleInstruction::REMOVE_LINE)]   |
	 change_mass[Process(self.d_p, self.d_s, SimpleInstruction::CHANGE_MASS)]   |
	 change_fluctuation_delta[Process(self.d_p, self.d_s,
			      SimpleInstruction::CHANGE_FLUCTUATION_DELTA)] |
	 change_fluctuation_start[Process(self.d_p, self.d_s,
			      SimpleInstruction::CHANGE_FLUCTUATION_START)] |
	 change_ks[Process(self.d_p, self.d_s, SimpleInstruction::CHANGE_KS)]       |
	 change_kd[Process(self.d_p, self.d_s, SimpleInstruction::CHANGE_KD)]       |
	 move_to[Process(self.d_p, self.d_s, SimpleInstruction::MOVE_TO)]           |
	 nop[Process(self.d_p, self.d_s, SimpleInstruction::NOP)]
        );

      return;
    }
    ~definition(void) { return; }
  };
};

class SimpleParser : public Parser {
public:
  SimpleParser(void) { return; }
  ~SimpleParser(void) { return; }

protected:
  State *NewState(void) const {
    return new SimpleState(*this);
  }

  class SimpleState : public State {
  public:
    SimpleState(SimpleParser &p) : d_grammar(p, *this) { return; }

    SimpleGrammar d_grammar;

    unsigned int d_pointId, d_point2Id;
    double d_mass;
    double d_ks, d_kd;
    double d_fluctuationDelta, d_fluctuationStart;
    double d_x, d_y, d_z;
    int d_instruction;
  };

  bool Parse(State &state, const std::string &str) const {
    SimpleState &s = (SimpleState &)state;

    parse_info<string::const_iterator> info;
    info = parse(str.begin(), str.end(), s.d_grammar, space_p);

    if(info.full) {
      return true;
    }
    return false;
  }

  void CreateInstruction(State &state) const {
    SimpleState &s = (SimpleState &)state;
    if(s.d_validate) { return; }

    Instruction *i;
    switch(state.d_instruction) {
    case SimpleInstruction::SPLIT:
      i = new Split(s.d_pointId, s.d_mass, s.d_x, s.d_y, s.d_z, s.d_ks, s.d_kd,
		    s.d_fluctuationDelta, s.d_fluctuationStart);
      break;
    case SimpleInstruction::SPLIT_DUP:
      i = new SplitDup(s.d_pointId, s.d_mass, s.d_x, s.d_y, s.d_z, s.d_ks,
		       s.d_kd, s.d_fluctuationDelta, s.d_fluctuationStart);
      break;
    case SimpleInstruction::MERGE:
      i = new Merge(s.d_pointId, s.d_point2Id);
      break;
    case SimpleInstruction::REMOVE_POINT:
      i = new RemovePoint(s.d_pointId);
      break;
    case SimpleInstruction::ADS.D_LINE:
      i = new AddLine(s.d_pointId, s.d_ks, s.d_kd,
		      s.d_fluctuationDelta, s.d_fluctuationStart);
      break;
    case SimpleInstruction::REMOVE_LINE:
      i = new RemoveLine(s.d_pointId);
      break;
    case SimpleInstruction::CHANGE_MASS:
      i = new ChangeMass(s.d_mass);
      break;
    case SimpleInstruction::CHANGE_KS:
      i = new ChangeKS(s.d_pointId, s.d_ks);
      break;
    case SimpleInstruction::CHANGE_KD:
      i = new ChangeKD(s.d_pointId, s.d_kd);
      break;
    case SimpleInstruction::CHANGE_FLUCTUATION_START:
      i = new ChangeFluctuationStart(s.d_pointId, s.d_fluctuationStart);
      break;
    case SimpleInstruction::CHANGE_FLUCTUATION_DELTA:
      i = new ChangeFluctuationDelta(s.d_pointId, s.d_fluctuationDelta);
      break;
    case SimpleInstruction::MOVE_TO:
      i = new MoveTo(s.d_pointId);
      break;
    case SimpleInstruction::NOP:
      i = new Nop;
      break;
    default: s.d_lineErrors.push_back(s.d_instructionLine); return;
    }

    if(i == NULL) {
      s.d_lineErrors.push_back(s.d_instructionLine);
    } else {
      s.d_instructions.push_back(i);
    }

    s.d_instructionLine++;
    return;
  }

private:
  void Update(SimpleState &s, int which, double value) const {
    if(s.d_validate) { return; }

    switch(which) {
    case SimpleInstruction::X: s.d_x = value; return;
    case SimpleInstruction::Y: s.d_y = value; return;
    case SimpleInstruction::Z: s.d_z = value; return;
    case SimpleInstruction::MASS: s.d_mass = value; return;
    case SimpleInstruction::KS: s.d_ks = value; return;
    case SimpleInstruction::KD: s.d_kd = value; return;
    case SimpleInstruction::FDELTA: s.d_fluctuationDelta = value; return;
    case SimpleInstruction::FSTART: s.d_fluctuationStart = value; return;
    default: s.d_lineErrors.push_back(s.d_instructionLine);
    }

    return;
  }
  void Update(SimpleState &s, int which, unsigned int value) const {
    if(s.d_validate) { return; }

    switch(which) {
    case SimpleInstruction::ID1: s.d_pointId = value; return;
    case SimpleInstruction::ID2: s.d_point2Id = value; return;
    default: s.d_lineErrors.push_back(s.d_instructionLine);
    }

    return;
  }

  friend class Process;
  friend class SimpleGrammar;
  friend class SimpleGrammar::definition;
};

#endif
