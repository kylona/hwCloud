#ifndef SCHEME_H
#define SCHEME_H

#include <string>
#include <sstream>
#include "Relation.h"
#include "datalogObject.h"

class Scheme : public DatalogObject {
public:
  std::string toString() {
    std::stringstream ss;
    ss << DatalogObject::toString() << std::endl;
    return ss.str();
  }

  Relation* toRelation() {
    return predicate.toRelation();
  }
};
#endif
