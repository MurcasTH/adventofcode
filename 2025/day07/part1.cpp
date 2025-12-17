#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>


int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cout << "Could not open file" << std::endl;
    return 1;
  }

  char laser = '|';
  char empty = '.';
  char splitter = '^';
  char emitter = 'S';
  
  std::vector<std::vector<char>> tacheonDiagram;
  std::string line;
  while (std::getline(file, line)) {
    std::vector<char> temp;
    for (uint64_t i = 0; i < line.length(); i++) {
      temp.push_back(line[i]);
    }
    tacheonDiagram.push_back(temp);
  }
  uint64_t splittCount = 0;
  for (uint64_t i = 0; i < tacheonDiagram.size()-1; i++) {
    int sectionLength = tacheonDiagram[i].size();
    for (int k = 0; k < sectionLength; k++) {
      if ((tacheonDiagram[i][k] == emitter || tacheonDiagram[i][k] == laser) && tacheonDiagram[i+1][k] == empty) {
        tacheonDiagram[i+1][k] = laser;
      }

      if(tacheonDiagram[i][k] == splitter && tacheonDiagram[i-1][k] == laser) {
        splittCount++;
        if (tacheonDiagram[i][k-1] == empty) {
          tacheonDiagram[i][k-1] = laser;
          tacheonDiagram[i+1][k-1] = laser;
        }
        if (tacheonDiagram[i][k+1] == empty) {
          tacheonDiagram[i][k+1] = laser;
          tacheonDiagram[i+1][k+1] = laser;
        }

      }
    }
  }

  for (std::vector<char> section : tacheonDiagram) {
    for (char symbol : section) {
      std::cout << symbol;
    }
    std::cout << std::endl;
  }
  
  std::cout << splittCount << std::endl;

  return 0;
}


