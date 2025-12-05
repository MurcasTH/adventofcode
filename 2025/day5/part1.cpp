#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string s, std::string del);

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cout << "File could not open" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<long unsigned int> ranges;
  std::vector<long unsigned int> IDs;
  int freshIDs = 0;

  bool lineIsID = false;
  while (std::getline(file, line)) {
    if (line.empty()) {
      lineIsID = true;
      continue;
    }
    
    if (lineIsID) {
      IDs.push_back(std::stoul(line));
      continue;
    }

    std::vector<std::string> stringNumbers = split(line, "-");
    for(std::string stringNum : stringNumbers) {
      ranges.push_back(std::stoul(stringNum));
    }
  }

  for (long unsigned int i = 0; i < IDs.size(); i++) {
    bool isFresh = false;
    for (long unsigned int j = 0; j < ranges.size()/2; j++) {
      if (IDs[i] >= ranges[j*2] && IDs[i] <= ranges[j*2 + 1]) {
        isFresh = true;
        break;
      }
    }
    if (isFresh) {
      freshIDs++;
      continue;
    }
  }

  std::cout << freshIDs << std::endl;

  return 0;
}


std::vector<std::string> split(std::string s, std::string del) {
  std::vector<std::string> tokens;
  std::string token;
  size_t pos = 0;
  while ((pos = s.find(del)) != std::string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(token);
    s.erase(0, pos + del.length());
  }
  tokens.push_back(s);
  return tokens;
}
