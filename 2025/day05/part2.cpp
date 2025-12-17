#include <algorithm>
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
  std::vector<std::pair<long unsigned int, long unsigned int>> ranges;

  while (std::getline(file, line)) {
    if (line.empty()) {
      break;
    }
    
    std::vector<std::string> stringNumbers = split(line, "-");
    ranges.push_back({std::stoul(stringNumbers[0]), std::stoul(stringNumbers[1])});
  }
  
  std::sort(ranges.begin(), ranges.end(),
            [](const auto &a, const auto &b){
              return a.first < b.first;
  });

  std::vector<std::pair<long unsigned int, long unsigned int>> merged;
  merged.push_back(ranges[0]);
  
  for (long unsigned int i = 1; i < ranges.size(); i++) {
    auto &last = merged.back();
   if (ranges[i].first <= last.second + 1) {
      if (ranges[i].second > last.second) {
        last.second = ranges[i].second;
      }
    } else {
      merged.push_back(ranges[i]);
    }
  }
  
  long unsigned int sum = 0;

  for (const auto range : merged) {
    sum += range.second - range.first + 1;
  }

  std::cout << sum << std::endl;

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
