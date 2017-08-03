#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include "Parameter.h"
#include "Relation.h"
#include "TwoWayMap.h"

class Predicate {
protected:
  std::vector<Parameter*> parameters;

public:
  virtual ~Predicate() {
    while(!parameters.empty()) {
      delete parameters.back();
      parameters.pop_back();
    }
  }

  Token* getDataAt(int index) {
    parameters.at(index)->getData();
  }

  virtual void addParameter(Parameter* thing) {
    parameters.push_back(thing);
  }

  Relation* toRelation() {
    TwoWayMap<int, std::string> schema;
    if (parameters.size() == 0) throw "ERROR no parameters";
    for (int i = 1; i < parameters.size(); i++) {
      schema.insert(i-1, parameters[i]->toString());
    }
    return new Relation(parameters[0]->toString(),schema);
  }

  virtual std::string getName() {
    return parameters[0]->toString();
  }

  int getSize() {
    return parameters.size();
  }

  virtual std::string toString() {
    if (parameters.size() == 0) return "ERROR no parameters";
    std::stringstream ss;
    ss << parameters[0]->toString() << "("; //the name
    ss << parameters[1]->toString(); //first thing
    for (int i = 2; i < parameters.size(); i++) {
      ss << "," << parameters[i]->toString(); //comma then next thing until done
    }
    ss << ")"; //close it off
    return ss.str();
  }
};

#endif
