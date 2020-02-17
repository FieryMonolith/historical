#include "setting_parser.h"

using namespace std;

SettingParser::SettingParser(void) {
  return;
}

SettingParser::~SettingParser(void) {
  return;
}

bool SettingParser::ParseVariable(istream &in,
				  const map<string,InputBase *> &rules) {
  string variableName;
  in >> ws;
  in >> variableName;
  in >> ws;

  if(variableName.empty()) { return true; }
  //if(!in.good()) { return false; }
  if(!in.good()) { return true; }

  // Disregard the rest of the line if a comment symbol is found
  if(variableName.at(0) == '#') {
    char line[1024];
    in.getline(line, 1024);
    return true;
  }

  map<string, InputBase *>::const_iterator i = rules.find(variableName);
  //if(i == rules.end()) { return false; }
  if(i == rules.end()) { return true; }

  // Returns false if the data could not be retrieved
  if(!i->second->GetData(in)) { return false; }

  return true;
}

bool SettingParser::ParseStream(istream &in,
				const map<string, InputBase *> &rules) {
  while(!in.eof()) {
    // If the error is not eof
    if(!in.good()) { return false; }

    if(!ParseVariable(in, rules)) {
      in.clear();
      return false;
    }
  }

  return true;
}
