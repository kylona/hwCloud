#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "StateMachine.h"
#include "Token.h"

class Tokenizer {
private:
  std::vector<StateMachine*> SMachines;
  std::vector<Token*> tokens;

  int testMachine(StateMachine*, std::ifstream&, std::string&, int&);
  void testBetterMatch(StateMachine*, std::ifstream& in_file, int, int&, int&, std::string, std::string&);

public:
  void clearTokens();
  Tokenizer ();
  virtual ~Tokenizer () {
    while (!SMachines.empty()) {
      delete SMachines.back();
      SMachines.pop_back();
    }
  }
  std::vector<Token*> tokenize(std::ifstream& in_file);
};
#endif
