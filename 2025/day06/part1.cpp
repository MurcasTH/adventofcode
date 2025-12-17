#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>

struct mathProblems {
  int operand1;
  int operand2;
  int operand3;
  int operand4;
  char operation;
};

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cout << "Could not open file" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<std::string> stringbank;
  while (std::getline(file, line)) {
    stringbank.push_back(line);
  }
  std::vector<bool> isSpace;
  for (unsigned long i = 0; i < stringbank.size(); i++) {
    isSpace.push_back(false);
  }

  std::vector<mathProblems> problems;
  int lastPos = 0;
  unsigned long stringlength = stringbank[0].length();

  for (unsigned long j = 0; j <= stringlength; j++) {
    if ( (j == stringlength || stringbank[0][j] == ' ') &&
         (j == stringlength || stringbank[1][j] == ' ') &&
         (j == stringlength || stringbank[2][j] == ' ') &&
         (j == stringlength || stringbank[3][j] == ' ')  )   {
      int digits = j - lastPos;
      mathProblems temp;
      temp.operand1 = stoi(stringbank[0].substr(lastPos, digits));
      temp.operand2 = stoi(stringbank[1].substr(lastPos, digits));
      temp.operand3 = stoi(stringbank[2].substr(lastPos, digits));
      temp.operand4 = stoi(stringbank[3].substr(lastPos, digits));
      for (int i = 0; i < digits; i++) {
       char symbol = stringbank[4].substr(lastPos, digits)[i];
        if (symbol != ' ') {
          temp.operation = symbol;
        }
      }
      problems.push_back(temp);
      lastPos = j + 1;
    }
  }
  
  std::uint64_t sum = 0;
  for (mathProblems problem : problems) {
    if (problem.operation == '*') {
      sum += 1LL * problem.operand1 * problem.operand2 * problem.operand3 * problem.operand4;
    }
    else if (problem.operation == '+') {
      sum += problem.operand1 + problem.operand2 + problem.operand3 + problem.operand4;
    }
  }



  //for (const auto &p : problems) {
  //  std::cout << p.operand1 << " "
  //            << p.operand2 << " "
  //            << p.operand3 << " "
  //            << p.operand4 << " "
  //            << p.operation  << "\n";
  //}

  std::cout << sum << std::endl;
  return 0;
}
