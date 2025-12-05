#include <fstream>
#include <iostream>
#include <vector>
#include <string>

typedef long unsigned int luint;

int main (int argc, char *argv[]) {
  
  char paper = '@';
  luint minrange = -1;
  luint maxrange = 1;
  luint maxSurroundingRolls = 4;

  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cout << "could not open file" << std::endl;
    return 1;
  }

  std::vector<std::vector<char>> paperTable;
  std::string line;
  while (std::getline(file, line)) {
    std::vector<char> tempLine;
    for (luint i = 0; i < line.length(); i++) {
      tempLine.push_back(line[i]);
    }
    paperTable.push_back(tempLine);
  }
  
  int forkliftAccessable = 0;

  for (luint i = 0; i < paperTable.size(); i++) {
    for (luint j = 0; j < paperTable[i].size(); j++) {
      if (paperTable[i][j] != paper) continue;
      luint paperCount = 0;
      for (int k = minrange; k <= (int) maxrange; k++) {
        luint newYindex = i + k;
        if (newYindex < 0 || newYindex > paperTable.size() - 1) continue;

        for (int l = minrange; l <= (int) maxrange; l++) {
          if (l == 0 && k == 0) continue;
          luint newXindex = j + l;
          if (newXindex < 0 || newXindex > paperTable[i].size() - 1) continue;

          if (paperTable[newYindex][newXindex] == paper) paperCount++;
        }
      }
      if (paperCount < maxSurroundingRolls) forkliftAccessable++;
    }
  }

  std::cout << forkliftAccessable << std::endl;
  
  return 0;
}

