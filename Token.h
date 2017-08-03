#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <sstream>
#include <iostream>

typedef enum {UNDEFINED = -1,COMMA,PERIOD,Q_MARK,LEFT_PAREN,RIGHT_PAREN,COLON,COLON_DASH,MULTIPLY,ADD,
              SCHEMES,FACTS,RULES,QUERIES,ID,COMMENT,STRING,WHITESPACE,END_FILE} macType;

class Token {
private:
  std::string data;
  std::string error;
  int line;
  int type;

public:
  friend class Parser;
  Token (int type, std::string data, int line) {
    this->line = line;
    this->type = type;
    this->data = data;
    this->error = "";
  }

  Token* addError(std::string error) {
    this->error = error;
    return this;
  }
  int getType() const {
    return type;
  }
  int getLine() const {
    return line;
  }
  std::string getData() const {
    return data;
  }
  std::string toString() {
    std::stringstream ss;
    ss << "(";
    switch (type) {
      case UNDEFINED: ss << "UNDEFINED"; break;
      case COMMA: ss << "COMMA"; break;
      case PERIOD: ss << "PERIOD"; break;
      case Q_MARK: ss << "Q_MARK"; break;
      case LEFT_PAREN: ss << "LEFT_PAREN"; break;
      case RIGHT_PAREN: ss << "RIGHT_PAREN"; break;
      case COLON: ss << "COLON"; break;
      case COLON_DASH: ss << "COLON_DASH"; break;
      case MULTIPLY: ss << "MULTIPLY"; break;
      case ADD: ss << "ADD"; break;
      case SCHEMES: ss << "SCHEMES"; break;
      case FACTS: ss << "FACTS"; break;
      case RULES: ss << "RULES"; break;
      case QUERIES: ss << "QUERIES"; break;
      case ID: ss << "ID"; break;
      case COMMENT: ss << "COMMENT"; break;
      case STRING: ss << "STRING"; break;
      case END_FILE: ss << "EOF"; break;
      case WHITESPACE:return ""; break;
    }
  ss << ",\"" << data << "\"," << line << ")" << std::endl;
  return ss.str();
  }
};

#endif
