#include "Relation.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>

void Relation::optimize() {
  newData = data;
}

void Relation::merge(Relation* input) {
  for (auto item : input->data) {
    data.insert(item);
  }
  newData = input->data;
}

void Relation::antiProject(std::string what) {
    schema.remove(what);
}

TwoWayMap<int, std::string> Relation::generateSchema(Relation* check, std::map<std::string, std::vector<int>>& matchedID) {
  TwoWayMap<int, std::string> babySchema = schema; //put all in receiveing in
  for (auto id : check->schema.schema) {
    try {
      matchedID[id.second].push_back(babySchema.at(id.second));//its allready in there
      matchedID[id.second].push_back(id.first); //save match it for later.
    }
    catch (std::out_of_range) {
      babySchema.insert(babySchema.size(), id.second); //else put it in
    }
  }
  return babySchema;
}

void addData(bool addToData, std::vector<std::string>& checkTuple, std::set<int>& skipList, std::vector<std::string>& babyItem, std::set<std::vector<std::string>>& babyData) {
  if (addToData) {
    for (int i = 0; i < checkTuple.size(); i++) {
      if (!skipList.count(i)) babyItem.push_back(checkTuple[i]);
    }
    babyData.insert(babyItem);
  }
}

std::set<std::vector<std::string>> Relation::generateData(Relation* input, std::set<std::vector<std::string>> receivingData, std::set<std::vector<std::string>> inputData, std::map<std::string, std::vector<int>>& matchedID) {
  std::set<std::vector<std::string>> babyData;
  std::vector<std::string> babyItem;
  for (auto dataTuple : receivingData) {
    for (auto checkTuple : inputData) {
      babyItem = dataTuple;
      bool addToData = true;
      std::set<int> skipList;
      for (auto item : input->schema.schema) {
        if (matchedID.count(item.second)) { //if in both schemas
          if(checkTuple[matchedID.at(item.second)[1]] != dataTuple[matchedID.at(item.second)[0]] ) {
            //if we find a mis-match
            addToData = false;
            break; //skip to next tuple.
          }
          //erase the duplicate value
          skipList.insert(item.first);
        }
      }
      addData(addToData,checkTuple,skipList,babyItem,babyData);
    }
  }
  return babyData;
}


void Relation::naturalJoin(Relation* input) {
  Relation* corpse;
  std::map<std::string, std::vector<int>> matchedID;
  TwoWayMap<int, std::string> babySchema = generateSchema(input,matchedID);
  //std::set<std::vector<std::string>> babyData1 = generateData(input,data,input->newData,matchedID);
  std::set<std::vector<std::string>> babyData = generateData(input,data,input->data,matchedID);




  data = babyData;
//  for (auto item : babyData2) {
//    data.insert(item);
//  }
  schema = babySchema;
  std::cout << "";
}

Relation* Relation::select(int index, std::string what) {
  std::set<std::vector<std::string>> babyData;
  for (std::vector<std::string> const& item : data) {
    if (item[index] == what) babyData.insert(item);
  }
  return new Relation(name + "'",schema,babyData);
}

Relation* Relation::select(int index1, int index2) {
  std::set<std::vector<std::string>> babyData;
  for (std::vector<std::string> const& item : data) {
    if (item[index1] == item[index2]) babyData.insert(item);
  }
  return new Relation(name + "'",schema,babyData);
}



Relation* Relation::project(std::vector<int> list) {
  TwoWayMap<int, std::string> babySchema;
  std::set<std::vector<std::string>> babyData;
  int i = 0;
  for(auto item : list) {
    babySchema.insert(i,schema.at(item));
    i++;
  }
  try {
    for (std::vector<std::string> const& item : data) {
      std::vector<std::string> babyTuple;
      for (int i = 0; i < babySchema.size(); i++) {
        std::string idItem = babySchema.at(i);
        babyTuple.push_back(item[schema.at(idItem)]);
      }
      babyData.insert(babyTuple);
    }
  }
  catch(...) {
    throw "Tried to project an item not in schema";
  }
  return new Relation(name + "'",babySchema,babyData);

}

Relation* Relation::rename(std::string oldName, std::string newId) {
  Relation* baby = copy();
  try {
    schema.at(newId);
    //uncomment if forcing unique names
    //throw "New name must be unique";
  }
  catch(std::out_of_range) {

  }
  try {
    int index = schema.at(oldName);
    baby->schema.remove(oldName);
    baby->schema.insert(index,newId);
  }
  catch(...) {
    throw "Tried to remane a item that did not exist";
  }
  return baby;
}

std::string Relation::getSchemaAt(int index) {
  return schema.at(index);
}

int Relation::getSchemaAt(std::string name) {
  return schema.at(name);
}

Relation* Relation::copy() {
  return new Relation(name + "'",schema,data);
}

void Relation::addTuple(std::vector<std::string> datum) {
  data.insert(datum);
}

std::string Relation::toString(TwoWayMap<std::string,int> renameMap) {
  std::stringstream ss;
  if (data.size() == 0) {
    ss << "No" << std::endl;
  }
  else {
    ss << "Yes(" << data.size() << ")" << std::endl;
  }
  for (std::vector<std::string> const& item : data) {
    bool addComma = false;
    for (int i = 0; i < renameMap.size(); i++) {
      try {
        std::string newName = renameMap.getFirst(i);
        int newIndex = schema.at(newName);
        if (addComma) {
          ss << ", ";
        }
        else {
          ss << "  ";
        }
        ss << newName << "=" << item.at(newIndex);
        addComma = true;
      }
      catch (std::out_of_range) {

      }
    }
    if (addComma) ss << std::endl; //if we didn't print any don't new line
  }
  return ss.str();
}

std::string Relation::toPrettyString() {
  std::stringstream ss;
  ss << name << ":\n" << std::endl;
  for (int i = 0; i < schema.size(); i++) {
    ss << "|" << std::left << std::setw(15) << schema.at(i);
  }
  ss << "|" << std::endl << " ";
  for (int i = 0; i < schema.size(); i++) {
    ss << "----------------";
  }
  ss << std::endl;
  for (auto iter = data.begin(); iter != data.end(); iter++) {
    auto datum = *iter;
    for (int i = 0; i < datum.size(); i++) {
      ss << "|" << std::left << std::setw(15) << datum[i];
    }
    ss << "|" << std::endl;

  }
  return ss.str();
}
