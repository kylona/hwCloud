#ifndef RELATION_H
#define RELATION_H

#include <set>
#include <vector>
#include <string>
#include <sstream>

#include "TwoWayMap.h"

typedef enum {GET,RENAME,PROJECT,SELECT} enumCommand;

class Relation {
private:
  std::string name;
  TwoWayMap<int, std::string> schema;
  std::set<std::vector<std::string>> data;
  std::set<std::vector<std::string>> newData;
  void antiProject(std::string);
  TwoWayMap<int, std::string> generateSchema(Relation* check, std::map<std::string, std::vector<int>>& matchedID);
  std::set<std::vector<std::string>> generateData(Relation* input, std::set<std::vector<std::string>> receivingData, std::set<std::vector<std::string>> inputData, std::map<std::string, std::vector<int>>& matchedID);


public:
  Relation(std::string name, TwoWayMap<int, std::string> schema) {
    this->name = name;
    this->schema = schema;
  }

  Relation(std::string name, TwoWayMap<int, std::string> schema, std::set<std::vector<std::string>> data) {
    this->name = name;
    this->schema = schema;
    this->data = data;
  }

  virtual ~Relation () {

  }

  int getSize() {
    return data.size();
  }
  void optimize();
  void addTuple(std::vector<std::string>);
  void merge(Relation* input);
  void naturalJoin(Relation* inptu);
  std::map<int, std::string> getSchema() {
    return schema.schema;
  }
  int getSchemaAt(std::string name);
  Relation* select(int index, std::string what);
  Relation* select(int index1, int index2);
  Relation* project(std::vector<int> list);
  Relation* rename(std::string oldName, std::string newId);
  Relation* copy();
  std::string toString(TwoWayMap<std::string, int> renameMap);
  std::string toPrettyString();
  std::string getSchemaAt(int index);

};
#endif
