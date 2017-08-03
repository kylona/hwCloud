#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <string>
#include <vector>
#include <cctype>
#include "Token.h"


class StateMachine {
protected:
  int type;
  int current;
  bool accepted;

public:
  friend class Tokenizer;

  StateMachine() {
  }
  StateMachine (int type);
  virtual ~StateMachine () {
  }

  virtual void clean();
  virtual bool feed(char c, bool& accepted) = 0;
  bool check(std::string input);
  bool getAccept();
};

class WhiteSpaceMachine : public StateMachine {
public:
  WhiteSpaceMachine() {
    type = WHITESPACE;
    current = 0;
    accepted = false;
  }
  virtual ~WhiteSpaceMachine() {}
  bool feed(char c, bool& acceptInput) {
    acceptInput = accepted;
    accepted = true;
    return (isspace(c));
  }
};

class OLCommentMachine : public StateMachine {
public:
  OLCommentMachine() {
    type = COMMENT;
    current = 0;
    accepted = false;
  }
  virtual ~OLCommentMachine() {}
  bool feed(char c, bool& acceptInput) {
    acceptInput = accepted;
    switch (current) {
      case 0:
        accepted = true;
        current = 1;
        return (c == '#'); break;
      case 1:
        accepted = true;
        acceptInput = accepted;
        current = 2;
        return (c != '|' && c != '\n'); break;
      case 2:
        accepted = true;
        acceptInput = accepted;
        current = 2;
        return (c != '\n'); break;
      }
      throw "MachineError";
    }
};

class MLCommentMachine : public StateMachine {
public:
  MLCommentMachine() {
    type = COMMENT;
    current = 0;
    accepted = false;
  }
  virtual ~MLCommentMachine() {}
  bool feed(char c, bool& acceptInput) {
    acceptInput = accepted;
    switch (current) {
      case 0:
        accepted = false;
        current = 1;
        return (c == '#'); break;
      case 1:
        accepted = false;
        current = 2;
        return (c == '|'); break;
      case 2:
        accepted = false;
        if (c == '|') {
          current = 3;
        }
        else {
          current = 2;
        }
        return true; break;
      case 3:
        accepted = true;
        if (c == '#') {
          current = 4;
        }
        else {
          current = 2;
        }
        return true; break;
      case 4:
        accepted = true;
        return false;
    }
    throw "MachineError";
  }
};

class StringMachine : public StateMachine {
public:
  StringMachine() {
    type = STRING;
    current = 0;
    accepted = false;
  }
  virtual ~StringMachine() {}
  bool feed(char c, bool& acceptInput) {
    acceptInput = accepted;
    switch (current) {
      case 0:
        accepted = false;
        current = 1;
        return (c == '\''); break;
      case 1:
        accepted = true;
        if (c == '\'') {
          current = 2;
        }
        else {
          current = 1;
        }
        return true; break;
      case 2:
        accepted = false;
        if (c == '\'') { //second in a row
          current = 1;
          return true;
        }
        else {
          return false;
        }
        break;
    }
    throw "MachineError";
  }
};

class IDMachine : public StateMachine {
public:
  IDMachine() {
    type = ID;
    current = 0;
    accepted = false;
  }
  virtual ~IDMachine() {}
  bool feed(char c, bool& acceptInput) {
    acceptInput = accepted;
    switch(current) {
      case 0:
        accepted = true;
        current = 1;
        return (isalpha(c)); break;
      case 1:
        accepted = true;
        current = 1;
        return (isalpha(c) or isdigit(c)); break;
    }
    throw "MachineError";
    }
};

class PatternMatchMachine : public StateMachine {
private:
  std::string pattern;
public:
  void clean() {
    current = -1;
    accepted = false;
  }
  PatternMatchMachine(std::string pattern, int type) {
    this->type = type;
    this->pattern = pattern;
    current = -1;
    accepted = false;
  }

  bool feed(char c, bool& acceptInput) {
    acceptInput = accepted;
    current ++;
    accepted = (current + 1 == pattern.length());
    if (current >= pattern.length()) return false;
    return (c == pattern[current]);
  }

};

#endif
