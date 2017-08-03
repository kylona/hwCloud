#ifndef DATALOG_PROGRAM_H
#define DATALOG_PROGRAM_H

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <stack>
#include "datalogObject.h"
#include "Relation.h"

class DatalogProgram {
private:
  DatalogObject* current;
  std::vector<Scheme*> schemes;
  std::vector<Fact*> facts;
  std::vector<Rule*> rules;
  std::vector<Query*> queries;
  std::set<std::string> domain;
  std::map<std::string, Relation*> relations;
  int getCardinality();

public:
  friend class Parser;
  DatalogProgram () {
    current = NULL;
  }
  virtual ~DatalogProgram () {
    clear();
  }

  Relation* getRelation(std::string name) {
    try {
      return relations.at(name);
    }
    catch(...) {
      return NULL;
    }
  }

  void populateRelations();
  void evaluateQueries();
  void clear();
  void addScheme(Scheme* input);
  void addFact(Fact* input);
  void addRule(Rule* input);
  void addQuery(Query* input);


  std::set<std::string> getDomain();
  std::string toString();



};
#endif
