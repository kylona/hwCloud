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
  std::set<std::vector<std::string>> allData;
  std::set<std::vector<std::string>> newData;
  void antiProject(std::string);
  TwoWayMap<int, std::string> generateSchema(Relation* check, std::vector<std::pair<int,int>>& matchedID);
  std::set<std::vector<std::string>> generateData(TwoWayMap< std::set<std::pair<std::string,int>>, std::set<std::vector<std::string>> >& answers, std::vector<std::pair<int,int>>& matchedID);
  std::set<std::set<std::pair<std::string,int>>> generateMatchList(std::vector<std::pair<int,int>>& matchedID);
  TwoWayMap< std::set<std::pair<std::string,int>>, std::set<std::vector<std::string>> > generateAnswers(Relation* input, std::set<std::set<std::pair<std::string,int>>>& matchList);


public:
  Relation(std::string name, TwoWayMap<int, std::string> schema) {
    this->name = name;
    this->schema = schema;
  }

  Relation(std::string name, TwoWayMap<int, std::string> schema, std::set<std::vector<std::string>> data) {
    this->name = name;
    this->schema = schema;
    this->allData = data;
  }

  virtual ~Relation () {

  }

  int getSize() {
    return allData.size();
  }
  void optimize();
  void addTuple(std::vector<std::string>);
  void merge(Relation* input);
  void naturalJoin(Relation* inptu);
  Relation* copyNewData(std::set<std::vector<std::string>> input) {
    return new Relation(name,schema,input);
  }
  TwoWayMap<int,std::string> getSchema() {
    return schema;
  }
  std::set<std::vector<std::string>>& getData() {
    return allData;
  };
  int getSchemaAt(std::string name);
  Relation* select(int index, std::string what);
  Relation* select(int index1, int index2);
  Relation* project(std::vector<int> list);
  Relation* rename(std::string oldName, std::string newId);
  Relation* copy();
  std::string toString(TwoWayMap<int, std::string> renameMap);
  std::string toPrettyString();
  std::string getSchemaAt(int index);

};
#endif
