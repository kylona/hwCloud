#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "datalogProgram.h"
#include <stack>
#include <vector>

class Parser {
private:
  DatalogProgram program;
  std::stack<Token*> input;

  void check(int);
  void check(int, DatalogObject*);

  void scheme();
  void schemeList();
  void idList(DatalogObject*);
  void fact();
  void factList();
  void rule();
  void ruleList();
  void headPredicate(DatalogObject*);
  void predicate(DatalogObject*);
  void predicateList(DatalogObject*);
  void parameter(DatalogObject*);
  void parameterList(DatalogObject*);
  void expression(DatalogObject*);
  void myOperator(DatalogObject*);
  void query();
  void queryList();
  void stringList(DatalogObject*);

public:
  Parser () {
  }
  virtual ~Parser () {
  }

  DatalogProgram* test(std::vector<Token*> tokens);
  std::string toString() {
    return program.toString();
  }

};

#endif
