#include "Relation.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>

void Relation::optimize() {
  newData = allData;
}

void Relation::merge(Relation* input) {
  for (auto item : input->getData()) {
    allData.insert(item);
  }
  newData = input->getData();
}

void Relation::antiProject(std::string what) {
    schema.remove(what);
}

TwoWayMap<int, std::string> Relation::generateSchema(Relation* check, std::vector<std::pair<int,int>>& matchedID) {
  TwoWayMap<int, std::string> babySchema = schema; //put all in receiveing in
  for (auto id : check->getSchema().toVector()) {
    try {
      int Index1 = babySchema.at(id.second); //if its allready in there
      matchedID.push_back({Index1,id.first});
    }
    catch (std::out_of_range) {
      babySchema.insert(babySchema.size(), id.second); //else put it in
    }
  }
  return babySchema;
}

std::set<std::set<std::pair<std::string,int>>> Relation::generateMatchList(std::vector<std::pair<int,int>>& matchedID) {
  std::set<std::set<std::pair<std::string,int>>> matchList;
  for (auto dataTuple : getData()) {
    std::set<std::pair<std::string,int>> match;
    for (auto idSet : matchedID) {
      match.insert({dataTuple.at(idSet.first),idSet.second});
    }
    matchList.insert(match);
  }
  return matchList;
}

void addData(bool addToData, std::vector<std::string>& checkTuple, std::set<int>& skipList,std::set<std::pair<std::string,int>>& match, TwoWayMap< std::set<std::pair<std::string,int>>, std::set<std::vector<std::string>> >& answers) {
  std::set<std::vector<std::string>> babyData;
  std::vector<std::string> babyItem;
  if (addToData) {
    for (int i = 0; i < checkTuple.size(); i++) {
      if (!skipList.count(i)) babyItem.push_back(checkTuple[i]);
    }
    try {
      std::set<std::vector<std::string>> oldStuff = answers.at(match);
      std::set<std::vector<std::string>> newStuff = oldStuff;
      newStuff.insert(babyItem);
      answers.replace(oldStuff, newStuff);
    }
    catch (std::out_of_range) {
      answers.insert(match,std::set<std::vector<std::string>> {babyItem});
    }
  }

}

TwoWayMap< std::set<std::pair<std::string,int>>, std::set<std::vector<std::string>> > Relation::generateAnswers(Relation* input, std::set<std::set<std::pair<std::string,int>>>& matchList) {
  //maps question                   toAnswer
  TwoWayMap< std::set<std::pair<std::string,int>>, std::set<std::vector<std::string>> > answers;
  std::set<int> skipList;
  std::set<std::vector<std::string>> babyData;

  for (auto inputTuple : input->getData()) {
    for (auto match : matchList) {
      bool addToData = true;
      for (auto Test : match) {
        if (Test.first  != inputTuple.at(Test.second)){
          addToData = false;
          break;
        }
        skipList.insert(Test.second);
      }
      addData(addToData,inputTuple,skipList,match,answers);
    }
  }
  return answers;
}


std::set<std::vector<std::string>> Relation::generateData(TwoWayMap< std::set<std::pair<std::string,int>>, std::set<std::vector<std::string>> >& answers, std::vector<std::pair<int,int>>& matchedID) {
  std::set<std::vector<std::string>> babyData;
  for (auto dataTuple : getData()) {
    std::set<std::pair<std::string,int>> match; //generate match
    for (auto idSet : matchedID) {
      match.insert({dataTuple.at(idSet.first),idSet.second});
    }
    try { //if there is an answer to the match
      std::set<std::vector<std::string>> dataToAdd = answers.at(match); //look up data we need to add.
      for (auto appended : dataToAdd) { //for each vector we need to append
        std::vector<std::string> babyItem = dataTuple; //put all my data in baby.
        for (auto datum : appended) { //put extra data on the end
            babyItem.push_back(datum);
        }
        babyData.insert(babyItem);
      }
    }
    catch (std::out_of_range) {
      //if not in answers dont add it. do nothing
    }


  }
  return babyData;
}

void Relation::naturalJoin(Relation* input) {
  Relation* corpse;
  std::vector<std::pair<int,int>> matchedID;
  TwoWayMap<int, std::string> babySchema = generateSchema(input,matchedID);
  std::set<std::set<std::pair<std::string,int>>> matchList = generateMatchList(matchedID);
  TwoWayMap< std::set<std::pair<std::string,int>>, std::set<std::vector<std::string>> > answers = generateAnswers(input, matchList);
  std::set<std::vector<std::string>> babyData = generateData(answers, matchedID);


  allData = babyData;
  schema = babySchema;
  std::cout << "";
}

Relation* Relation::select(int index, std::string what) {
  std::set<std::vector<std::string>> babyData;
  for (std::vector<std::string> const& item : getData()) {
    if (item[index] == what) babyData.insert(item);
  }
  return new Relation(name,schema,babyData);
}

Relation* Relation::select(int index1, int index2) {
  std::set<std::vector<std::string>> babyData;
  for (std::vector<std::string> const& item : getData()) {
    if (item[index1] == item[index2]) babyData.insert(item);
  }
  return new Relation(name,schema,babyData);
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
    for (std::vector<std::string> const& item : getData()) {
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
  return new Relation(name,babySchema,babyData);

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
  return new Relation(name,schema,getData());
}

void Relation::addTuple(std::vector<std::string> datum) {
  allData.insert(datum);
}

std::string Relation::toString(TwoWayMap<int, std::string> renameMap) {
  std::stringstream ss;
  if (allData.size() == 0) {
    ss << "No" << std::endl;
  }
  else {
    ss << "Yes(" << allData.size() << ")" << std::endl;
  }
  for (std::vector<std::string> const& item : allData) {
    bool addComma = false;
    for (auto nameThing : renameMap.toVector() ) {
      std::string newName = nameThing.second;
      try {
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
  for (auto iter = allData.begin(); iter != allData.end(); iter++) {
    auto datum = *iter;
    for (int i = 0; i < datum.size(); i++) {
      ss << "|" << std::left << std::setw(15) << datum[i];
    }
    ss << "|" << std::endl;

  }
  return ss.str();
}
