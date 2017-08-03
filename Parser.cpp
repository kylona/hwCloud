#include "Parser.h"

DatalogProgram* Parser::test(std::vector<Token*> tokens) {
  program.clear();
  for (int i = tokens.size(); i > 0; i--) {
    if (tokens[i - 1]->type != COMMENT) input.push(tokens[i - 1]);
  }
  check(SCHEMES);
  check(COLON);
  scheme();
  schemeList();
  check(FACTS);
  check(COLON);
  factList();
  check(RULES);
  check(COLON);
  ruleList();
  check(QUERIES);
  check(COLON);
  query();
  queryList();

  return &program;
}

void Parser::check(int goalType) {
  int inputType = input.top()->type;
  if (inputType != goalType) {
    //std::cout << "Expected: " << goalType << std::endl;
    throw(input.top());
  }
  else {
    input.pop();
  }
}

void Parser::check(int goalType, DatalogObject* storeTo) {
  int inputType = input.top()->type;
  if (inputType != goalType) {
    //std::cout << "Expected: " << goalType << std::endl;
    throw input.top();
  }
  else {
    storeTo->addThing(input.top());
    input.pop();
  }
}

void Parser::scheme() {
  program.addScheme(new Scheme());
  check(ID,program.current);
  check(LEFT_PAREN);
  check(ID,program.current);
  idList(program.current);
  check(RIGHT_PAREN);
}
void Parser::schemeList() {
  if (input.top()->type == FACTS) return;
  scheme();
  schemeList();
}

void Parser::idList(DatalogObject* storeTo) {
  if (input.top()->type == RIGHT_PAREN) return;
  check(COMMA);
  check(ID,storeTo);
  idList(storeTo);
}
void Parser::fact() {
  program.addFact(new Fact());
  check(ID,program.current);
  check(LEFT_PAREN);
  check(STRING,program.current);
  stringList(program.current);
  check(RIGHT_PAREN);
  check(PERIOD);

}
void Parser::factList() {
  if (input.top()->type == RULES) return;
  fact();
  factList();
}
void Parser::rule() {
  program.addRule(new Rule());
  headPredicate(program.current);
  check(COLON_DASH,program.current); //send COLON_DASH to mark end of head
  predicate(program.current);
  predicateList(program.current);
  check(PERIOD);
}
void Parser::ruleList() {
  if (input.top()->type == QUERIES) return;
  rule();
  ruleList();
}
void Parser::headPredicate(DatalogObject* storeTo) {
  check(ID,storeTo);
  check(LEFT_PAREN);
  check(ID,storeTo);
  idList(storeTo);
  check(RIGHT_PAREN);
}
void Parser::predicate(DatalogObject* storeTo) {
  check(ID,storeTo);
  check(LEFT_PAREN);
  parameter(storeTo);
  parameterList(storeTo);
  check(RIGHT_PAREN);
}
void Parser::predicateList(DatalogObject* storeTo) {
  if (input.top()->type == PERIOD) return;
  check(COMMA,storeTo);
  predicate(storeTo);
  predicateList(storeTo);
}
void Parser::parameter(DatalogObject* storeTo) {
  switch (input.top()->type) {
    case STRING: check(STRING,storeTo); break;
    case ID: check(ID,storeTo); break;
    case LEFT_PAREN: expression(storeTo); break;
    default : throw input.top();
  }
}
void Parser::parameterList(DatalogObject* storeTo) {
  if (input.top()->type == RIGHT_PAREN) return;
  check(COMMA);
  parameter(storeTo);
  parameterList(storeTo);
}
void Parser::expression(DatalogObject* storeTo) {
  check(LEFT_PAREN,storeTo); //left paren tells to store as expression not parameter
  parameter(storeTo);
  myOperator(storeTo);
  parameter(storeTo);
  check(RIGHT_PAREN,storeTo);
}
void Parser::myOperator(DatalogObject* storeTo) {
  switch (input.top()->type) {
    case ADD: check(ADD,storeTo); break;
    case MULTIPLY: check(MULTIPLY,storeTo); break;
    default : throw input.top();
  }
}
void Parser::query() {
  program.addQuery(new Query());
  predicate(program.current);
  check(Q_MARK);
}
void Parser::queryList() {
  if (input.top()->type == END_FILE) return;
  query();
  queryList();
}
void Parser::stringList(DatalogObject* storeTo) {
  if (input.top()->type == RIGHT_PAREN) return;
  check(COMMA);
  check(STRING,storeTo);
  stringList(storeTo);
}
