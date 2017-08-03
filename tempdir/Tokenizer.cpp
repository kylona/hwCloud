#include "Tokenizer.h"

void Tokenizer::clearTokens() {
  while (!tokens.empty()) {
    delete tokens.back();
    tokens.pop_back();
  }
}

Tokenizer::Tokenizer() {
  SMachines.push_back(new OLCommentMachine());
  SMachines.push_back(new MLCommentMachine());
  SMachines.push_back(new WhiteSpaceMachine());
  SMachines.push_back(new StringMachine());
  SMachines.push_back(new PatternMatchMachine("Schemes",SCHEMES));
  SMachines.push_back(new PatternMatchMachine("Facts",FACTS));
  SMachines.push_back(new PatternMatchMachine("Rules",RULES));
  SMachines.push_back(new PatternMatchMachine("Queries",QUERIES));
  SMachines.push_back(new IDMachine());
  SMachines.push_back(new PatternMatchMachine(",",COMMA));
  SMachines.push_back(new PatternMatchMachine(".",PERIOD));
  SMachines.push_back(new PatternMatchMachine("?",Q_MARK));
  SMachines.push_back(new PatternMatchMachine("(",LEFT_PAREN));
  SMachines.push_back(new PatternMatchMachine(")",RIGHT_PAREN));
  SMachines.push_back(new PatternMatchMachine(":",COLON));
  SMachines.push_back(new PatternMatchMachine(":-",COLON_DASH));
  SMachines.push_back(new PatternMatchMachine("*",MULTIPLY));
  SMachines.push_back(new PatternMatchMachine("+",ADD));
}

int Tokenizer::testMachine(StateMachine* now, std::ifstream& in_file, std::string& saveChar, int& LineNumber) {
  int n = 0;
  char c;
  bool accepted;
  while(true) {
    if (n >= saveChar.size()) {
      if (!in_file.get(c)) {
        accepted = now->getAccept();
        break;
      }
      saveChar += c;
      if (c == '\n') LineNumber++;
    }
    if (!now->feed(saveChar[n], accepted)) break;
    n++;
  }
  return (n*accepted); //if accepted false return 0; if true return n;
}

void Tokenizer::testBetterMatch(StateMachine* now, std::ifstream& in_file, int n, int& recon, int& longestRecon, std::string saveChar, std::string& bestData) {
  if (n <= longestRecon) return; // less or equal so ties go to first
  if (!in_file.eof()) saveChar.pop_back();  //if not at end drop last char (it was the failed char)
    recon = now->type;
    longestRecon = n;
    bestData = saveChar;
}

std::vector<Token*> Tokenizer::tokenize(std::ifstream& in_file) {
  clearTokens();
  int LineNumber = 1;
  int MatchLineNumber = 1;
  std::string saveChar;
  std::string bestData;
  int recon = UNDEFINED;
  do {
    if (!saveChar.empty()) saveChar = saveChar.back(); //keep the last char in the string (it still needs testing)
    int longestRecon = 0;
    recon = UNDEFINED;
    MatchLineNumber = LineNumber;
    for (int i = 0; i < SMachines.size(); i++) {
        StateMachine* now = SMachines[i];
        now->clean();
        int n = testMachine(now,in_file,saveChar,LineNumber);
        testBetterMatch(now,in_file,n,recon,longestRecon,saveChar,bestData);
    }
    if (recon == UNDEFINED) {
      bestData = saveChar;
      saveChar.clear();//clean out the unrecognized junk.
    }
    if (recon != WHITESPACE && bestData != "") {
      tokens.push_back(new Token(recon,bestData,MatchLineNumber));
    }

  } while (!in_file.eof());
  tokens.push_back(new Token(END_FILE,"",LineNumber));
  return tokens;
}
