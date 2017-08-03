#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <sstream>
#include <stdexcept>
#include "datalogObject.h"

const int SKIP_NAME = 1;

class Query : public DatalogObject {
private:
  Relation* activeR = NULL;
  Relation* corpse = NULL;
  std::vector<int> projectList;
  TwoWayMap<std::string,int> renameMap;

public:
  std::string toString() {
    std::stringstream ss;
    ss << DatalogObject::toString() << "?";
    if (activeR != NULL) ss << " " << activeR->toString(renameMap);
    return ss.str();
  }

  Relation* runQueryCommands(Relation* activeR) {
    this->activeR = activeR;
    runQueryCommand(0);
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
};
#endif
