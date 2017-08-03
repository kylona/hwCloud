#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Parameter.h"

class Expression : public Parameter {
private:
  Parameter* left;
  Token* myOperator;
  Parameter* right;
  Expression* parrent;
  Token* result;

public:
  Expression() {
    parrent = NULL;
    left = NULL;
    myOperator = NULL;
    right = NULL;
    result = NULL;
  }

  Expression(Expression* parrent) {
    this->parrent = parrent;
    left = NULL;
    myOperator = NULL;
    right = NULL;
    result = NULL;
  }

  ~Expression() {
    if (left != NULL) delete left;
    if (right != NULL) delete right;
    if (result != NULL) delete result;
  }

  Token* getData() {
    if (result != NULL) return result;
    if (myOperator->getType() == ADD) {
      Token* leftData = left->getData();
      Token* rightData= right->getData();
      if(leftData->getType() != rightData->getType()) throw "Mismatch type in expression";
      std::string rightString = rightData->getData();
      std::string leftString = leftData->getData();
      if(leftData->getType() == STRING) {//pop to make 'something''else' 'somethingelse'
        leftString.pop_back();
        rightString = rightString.erase(0,1);
      }
      std::string babyData = leftString + rightString;
      result = new Token(leftData->getType(),babyData,leftData->getLine());
      return result;
    }
    throw "Multiply not supported in this version";
  }

  Parameter* AddToExpr(Token* thing) {
    if (left == NULL) {
      if (thing->getType() == LEFT_PAREN) {
        left = new Expression(this);
        return left;
      }
      else {
        left = new Parameter(thing);
        return this;
      }
    }
    if (myOperator == NULL) {
      myOperator = thing;
      return this;
    }
    if (right == NULL) {
      if (thing->getType() == LEFT_PAREN) {
        right = new Expression(this);
        return right;
      }
      else {
        right = new Parameter(thing);
        return this;
      }
    }
    if (thing->getType() == RIGHT_PAREN) return parrent;
    throw (thing->addError("EXPRESSION ERROR"));
  }

  std::string toString() {
    if (left == NULL || right == NULL || myOperator == NULL) throw "INVALID EXPRESSION";
    std::stringstream ss;
    ss << "(" << left->toString() << myOperator->getData() << right->toString() << ")";
    return ss.str();
  }

};

#endif
