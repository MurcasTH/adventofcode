#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> split(std::string s, std::string del);
bool sequenceRepeatedTwice(int number);

int main (int argc, char *argv[]) {
  std::fstream file("input");
  if (!file.is_open()) {
    std::cerr << "File could not open" << std::endl;
    return 1;
  }
  std::string line;
  std::getline(file, line);

  //split the string into a vector containg smaller strings
  std::vector<std::string> rangesString = split(line, ",");

  //split the smaller strings into even smaller strings that can be converted to numbers
  std::vector<std::vector<std::string>> splitranges;
  for(std::string range : rangesString) {
    splitranges.push_back(split(range, "-"));
  }

  //convert the even smaller strings into numbers
  //the start of a range is at even indexes, the end of a range is at the odd.
  std::vector<int> ranges;
  for (std::vector<std::string> stringvector : splitranges) {
    int i = 0;
    std::array<int, 2> range;
    for (std::string s : stringvector) {
      ranges.push_back(std::stoi(s));
    }
  }

  return 0;
}

//function to split a string by a delimiter to a vector array
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

bool sequenceRepeatedTwice(int number) {
  std::string fullString = std::to_string(number);
  int fullLength = fullString.length();
  if(fullLength % 2 != 0) return false;

  std::string firstHalf = fullString.substr(0, fullLength/2 - 1);
  std::string lastHalf  = fullString.substr(fullLength/2, fullLength - 1);
  return firstHalf == lastHalf;
}


