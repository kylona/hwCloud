#ifndef RULE_H
#define RULE_H

#include "Token.h"
#include <vector>
#include <string>
#include <sstream>
#include "datalogObject.h"


class Rule : public DatalogObject {
private:
  std::vector<Predicate*> predicates;
  Relation* activeR = NULL;
  Relation* corpse = NULL;
  std::vector<int> projectList;
  TwoWayMap<std::string,int> renameMap;
public:
  Rule() {
    predicates.push_back(new Predicate());
  }
  ~Rule() {
    return;
    while(!predicates.empty()) {
      delete predicates.back();
      predicates.pop_back();
    }
  }

  void optimize(std::map<std::string, Relation*>& relations) {
    relations.at(predicates[0]->getName())->optimize();
    std::cout << "";
  }

  void toRelation(std::map<std::string, Relation*>& relations) {
    Relation* PastR = NULL;
    Relation* SaveToR = NULL;
    std::vector<int> projectList;
    for (int i = 1; i < predicates.size(); i++) {
      clear();
      this->predicate = *predicates[i];
      try {
        this->activeR = relations.at(predicate.getName())->copy();
        activeR = runQueryCommand(0);
        if (PastR != NULL) {
          PastR->naturalJoin(activeR);
        }
        else {
          PastR = activeR;
        }
      }
      catch (std::out_of_range) {
        throw "Rule not defined in schema";
      }
    }
    activeR = PastR;
      SaveToR = relations.at(predicates[0]->getName());
      for (int i = SKIP_NAME; i < predicates[0]->getSize(); i++) {
        std::string item = predicates[0]->getDataAt(i)->getData();
        projectList.push_back(activeR->getSchemaAt(item));
      }
      PastR = activeR;
      activeR = activeR->project(projectList);
      delete PastR;
      SaveToR->merge(activeR);
  }

  Relation* runQueryCommand(int param) {
    try {
      Token* data = predicate.getDataAt(param + SKIP_NAME);
      switch (data->getType()) {
        case STRING:
          corpse = activeR;
          activeR = activeR->select(param,data->getData());
          delete corpse;
          runQueryCommand(param + 1);
          break;
        case ID:
          std::string oldName = activeR->getSchemaAt(param);
          try { //if we have seen it before
            corpse = activeR;
            activeR = activeR->select(renameMap.at(data->getData()),param);
            delete corpse;
          }
          catch (std::out_of_range) { //if we have not seen it before
            renameMap.insert(data->getData(),param);
          }
          projectList.push_back(param);
          runQueryCommand(param + 1);
          corpse = activeR;
          activeR = activeR->rename(oldName,data->getData());
          delete corpse;
          break;
      }
      return activeR;
    }
    catch (std::out_of_range) {
      if (!projectList.empty()) {
        corpse = activeR;
        activeR = activeR->project(projectList);
        delete corpse;
      }
      return activeR;//once getDataAt fails
    }

  }

  void clear() {
    activeR = NULL;
    corpse = NULL;
    projectList.clear();
    renameMap.clear();
  }


  void addThing(Token* thing) {
    if (thing->getType() == COMMA || thing->getType() == COLON_DASH) {
      predicates.push_back(new Predicate());
    }
    else {
      DatalogObject::addThing(thing); //treat normaly
    }
  }
  void addThing(Parameter* thing) {
    predicates[predicates.size() - 1]->addParameter(thing); //save it to the newest predicate
  }
  std::string toString() {
    std::stringstream ss;
    ss << predicates[0]->toString() << " :- ";
    if (predicates.size() > 1) {
      ss << predicates[1]->toString(); //first thing
      for (int i = 2; i < predicates.size(); i++) {
        ss << "," << predicates[i]->toString(); //comma then next thing until done
      }
      ss << "." << std::endl; //close it off
    }
    return ss.str();
  }
};

#endif
