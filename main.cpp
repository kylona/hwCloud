#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include "Tokenizer.h"
#include "Parser.h"
#include "Relation.h"
#include "datalogProgram.h"

void checkFileType(std::string, DatalogProgram*);

void testLexer(std::ifstream& in_file) {
  Tokenizer lex;
  std::vector<Token*> tokens = lex.tokenize(in_file);
  for (int i = 0; i < tokens.size(); i++) {
    std::cout << tokens[i]->toString();
  }
  std::cout << "Total Tokens = " << tokens.size() << std::endl;
}

void testParser(std::ifstream& in_file) {
  Tokenizer lex;
  std::vector<Token*> tokens = lex.tokenize(in_file);
  Parser pars;
  pars.test(tokens);
  std::cout << "Success!" << std::endl;
  std::cout << pars.toString();
  lex.clearTokens();
}

void testRelation(std::ifstream& in_file) {
  Tokenizer lex;
  Parser pars;
  DatalogProgram* program;
  std::vector<Token*> tokens = lex.tokenize(in_file);
  program = pars.test(tokens);
  program->populateRelations();
  program->evaluateQueries();
  lex.clearTokens();
}

void checkFileType(std::string fileName, DatalogProgram* program = NULL) {
  std::ifstream in_file;
  in_file.open(fileName);
  if (!in_file.is_open()) {
    throw "File not found";
  }
  std::string extention = fileName.substr(fileName.length()-4,4);
    testRelation(in_file); return;
    if (extention == ".par") testParser(in_file);
    else if (extention == ".lex") testLexer(in_file);
    else if (extention == ".rel") testRelation(in_file);
    else testRelation(in_file); //default
}

int main(int argc, char* argv[]) {

  for (int a = 1; a < argc; a++) {
    try {
      checkFileType(argv[a]);
    }
    catch (char const* error) {
      std::cout << "Failure!" << std::endl << "  " << error << std::endl;
    }
    catch (Token* error) {
      std::cout << "Failure!" << std::endl << "  " << error->toString();
    }
  }
  return 0;
}
