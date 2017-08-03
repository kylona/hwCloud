#ifndef PARAMETER_H
#define PARAMETER_H

#include "Token.h"
#include <vector>
#include <string>
#include <sstream>

class Parameter {
private:
  Token* data;

public:
  Parameter (Token* data) {
    if (data->getType() != ID && data->getType() != STRING) throw data;
    this->data = data;
  }

  Parameter() {
    this->data = NULL;
  }

  virtual ~Parameter() {
  }

  virtual Parameter* AddToExpr(Token* thing) {
    throw "NOT AN EXPRESSION";
    return this;
  }

  virtual Token* getData() {
    return data;
  }

  int getType() {
    return data->getType();
  }

  virtual std::string toString() {
    return data->getData();
  }

};

#endif
