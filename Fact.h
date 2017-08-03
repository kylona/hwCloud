#ifndef FACT_H
#define FACT_H

#include <string>
#include <sstream>
#include <set>
#include <vector>
#include "datalogObject.h"

class Fact : public DatalogObject {
private:
  std::vector<std::string> domain;

public:
  void addThing(Parameter* thing) {
    if (thing->getType() == STRING) domain.push_back(thing->toString());
    DatalogObject::addThing(thing);
  }
  std::string toString() {
    std::stringstream ss;
    ss << DatalogObject::toString() << "." << std::endl;
    return ss.str();
  }

  std::vector<std::string> toTuple() {
    std::vector<std::string> aTuple;
    for (std::string item : domain) {
      aTuple.push_back(item);
    }
    return aTuple;
  }

  std::vector<std::string> getDomain() const {
    return domain;
  }

};
#endif
