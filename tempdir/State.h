#ifndef STATE_H
#define STATE_H

#include <map>
#include <iostream>
#include <cctype>

class State {
private:
  bool accept;
  State* defaultPath;
  std::map<char, State*> StateMap;

public:
  State () {
    accept = false;
    defaultPath = NULL;
  }
  virtual ~State() {
    clear();
  }

  void clear() {
    StateMap.clear();
    accept = false;
    defaultPath = NULL;
  }

  void noFail() {
    defaultPath = this;
  }

  virtual State* execute(char c) {
    try {
      return StateMap.at(c);
    }
    catch (...) {
      return defaultPath;
    }
  }

  void AddPath(char key, State* destination) {
    StateMap[key] = destination;
  }

  bool getAccept() const {
    return accept;
  }

  void setAccept(bool value) {
    accept = value;
  }

  void setDefaultPath(State* value) {
    defaultPath = value;
  }

};

#endif
