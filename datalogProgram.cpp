#include "datalogProgram.h"

int DatalogProgram::getCardinality() {
  int cardinality = 0;
  for (auto item : relations) {
    cardinality += item.second->getSize();
  }
  return cardinality;
}

void DatalogProgram::populateRelations() {
  for (int i = 0; i < schemes.size(); i++) {
    relations[schemes[i]->getName()] = schemes[i]->toRelation();
  }
  for (int i = 0; i < facts.size(); i++) {
    try {
      relations.at(facts[i]->getName())->addTuple(facts[i]->toTuple());
    }
    catch (...) {
      throw "Fact not defined in schema";
    }
  }
  int cardinality = getCardinality();
  int numIterations = 0;
  for (int i = 0; i < rules.size(); i++) {
    rules[i]->optimize(relations);
  }
  while (true) {
    numIterations ++;
    //if (numIterations % 5 == 0 || true) std::cout << numIterations << ", " << cardinality << std::endl;
    int previousSize = cardinality;
    for (int i = 0; i < rules.size(); i++) {
      rules[i]->toRelation(relations);
    }
    cardinality = getCardinality();
    if (previousSize == cardinality) break;
  }
  std::cout << "Schemes populated after " << numIterations << " passes through the Rules." << std::endl;
}

void DatalogProgram::evaluateQueries() {
  for (int i = 0; i < queries.size(); i++) {
    Relation* activeR;
    try {
      activeR = relations.at(queries[i]->getName());
    }
    catch (...) {
      throw "Query not defined in schema";
    }
      activeR = queries[i]->runQueryCommands(activeR->copy());
      std::cout << queries[i]->toString();
      delete activeR;
  }
}

void DatalogProgram::clear() {
  current = NULL;
  while(!schemes.empty()) {
    delete schemes.back();
    schemes.pop_back();
  }
  while(!facts.empty()) {
    delete facts.back();
    facts.pop_back();
  }
  while(!rules.empty()) {
    delete rules.back();
    rules.pop_back();
  }
  while(!queries.empty()) {
    delete queries.back();
    queries.pop_back();
  }
  for (auto item : relations) {
    delete item.second;
  }
  domain.clear();
}

void DatalogProgram::addScheme(Scheme* input) {
  current = input;
  schemes.push_back(input);
}
void DatalogProgram::addFact(Fact* input) {
  current = input;
  facts.push_back(input);
}
void DatalogProgram::addRule(Rule* input) {
  current = input;
  rules.push_back(input);
}
void DatalogProgram::addQuery(Query* input) {
  current = input;
  queries.push_back(input);
}



std::set<std::string> DatalogProgram::getDomain() {
  std::stringstream ss;
  for (int i = 0; i < facts.size(); i++) {
    std::vector<std::string> thisDomain = facts[i]->getDomain();
    domain.insert(thisDomain.cbegin(),thisDomain.cend());
  }
  return domain;
}

std::string DatalogProgram::toString() {
  std::stringstream ss;
  ss << "Schemes(" << schemes.size() << "):" << std::endl;
  for (int i = 0; i < schemes.size(); i++) {
    ss << "  " << schemes[i]->toString();
  }
  ss << "Facts(" << facts.size() << "):" << std::endl;
  for (int i = 0; i < facts.size(); i++) {
    ss << "  " << facts[i]->toString();
  }
  ss << "Rules(" << rules.size() << "):" << std::endl;
  for (int i = 0; i < rules.size(); i++) {
    ss << "  " << rules[i]->toString();
  }
  ss << "Queries(" << queries.size() << "):" << std::endl;
  for (int i = 0; i < queries.size(); i++) {
    ss << "  " << queries[i]->toString() << std::endl;
  }
  ss << "Domain(" << getDomain().size() << "):" << std::endl;
  for(std::string const& item: getDomain()) {
    ss << "  " << item << std::endl;
  }
  return ss.str();
}
