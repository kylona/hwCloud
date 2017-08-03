#include "StateMachine.h"
#include <sstream>

bool StateMachine::getAccept() {
  return accepted;
}

bool StateMachine::check(std::string input) {
  clean();
  bool accepted;
  for (int c = 0; c < input.length(); c++) {
    if (!feed(c,accepted)) return false;
  }
  return accepted;
}

void StateMachine::clean() {
  current = 0;
  accepted = false;
}
