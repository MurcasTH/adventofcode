#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>

struct mathProblems {
  std::vector<int> operands;
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

  std::vector<mathProblems> problems;
  int lastPos = 0;
  unsigned long stringlength = stringbank[0].length();
  std::vector<int> numbers;

  for (unsigned long j = 0; j <= stringlength; j++) {
    if ((j == stringlength || stringbank[0][j] == ' ') &&
        (j == stringlength || stringbank[1][j] == ' ') &&
        (j == stringlength || stringbank[2][j] == ' ') &&
        (j == stringlength || stringbank[3][j] == ' ')  )   {
      int digits = j - lastPos;
      mathProblems temp;
      for (const auto number : numbers) {
        temp.operands.push_back(number);
      }
      numbers.clear();
      for (int i = 0; i < digits; i++) {
       char symbol = stringbank[4].substr(lastPos, digits)[i];
        if (symbol != ' ') {
          temp.operation = symbol;
        }
      }
      problems.push_back(temp);
      lastPos = j + 1;
    } else {
      std::string stringNum;
      stringNum.push_back(stringbank[0][j]);
      stringNum.push_back(stringbank[1][j]);
      stringNum.push_back(stringbank[2][j]);
      stringNum.push_back(stringbank[3][j]);
      numbers.push_back(std::stoi(stringNum));
    }
    
  }
  
  std::uint64_t sum = 0;
  for (mathProblems problem : problems) {
    if (problem.operation == '*') {
      uint64_t product = 1;
      for (const auto operand : problem.operands) {
        product *= operand;
      }
      sum += product;
    }
    else if (problem.operation == '+') {
      for (const auto operand : problem.operands) {
        sum += operand;
      }
    }
  }



  for (mathProblems problem : problems) {
    for (const auto operand : problem.operands) {
      std::cout << operand << " ";
    }
    std::cout << problem.operation << std::endl;
  }

  std::cout << sum << std::endl;
  return 0;
}
