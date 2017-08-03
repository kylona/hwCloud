#ifndef DATALOG_OBJECT_H
#define DATALOG_OBJECT_H

#include "Predicate.h"
#include "Parameter.h"
#include "Expression.h"
#include "Token.h"

class DatalogObject {

protected:
  Predicate predicate; //for rule this is the headPredicate
  Parameter* storeTo = NULL;

  virtual void placeParameter(Token* thing) {
    switch (thing->getType()) {
      case ID: addThing(new Parameter(thing)); break;
      case STRING: addThing(new Parameter(thing)); break;
      case LEFT_PAREN:
      {
        storeTo = new Expression();
        addThing(storeTo);
        break;
      }
    }
  }

public:

  virtual ~DatalogObject () {

  }

  virtual void addThing(Token* thing) {
    if (storeTo == NULL) {
      placeParameter(thing);
    }
    else {
      storeTo = storeTo->AddToExpr(thing);
    }
  }

  virtual void addThing(Parameter* thing) {
    predicate.addParameter(thing);
  }

  virtual std::string toString() {
    return predicate.toString();
  }

  virtual std::string getName() {
    return predicate.getName();
  }
};

#include "Scheme.h"
#include "Fact.h"
#include "Query.h"
#include "Rule.h" //putting them here so they tag along

#endif
