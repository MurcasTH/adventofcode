#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> split(std::string s, std::string del);
bool sequenceRepeatedTwice(unsigned long long number);
bool sequenceRepeatedAtLeastTwice(unsigned long long number);

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
  std::vector<std::array<unsigned long, 2>> ranges;
  for (std::vector<std::string> stringvector : splitranges) {
    std::array<unsigned long, 2> range;
    int i = 0;
    for (std::string s : stringvector) {
      if (i>=2) break;
      range[i] = std::stoull(s);
      i++;
    }
    if (i == 2) ranges.push_back(range);
  }
  
  unsigned long long sum = 0;
  for (std::array<unsigned long, 2> range : ranges) {
    for (unsigned long long i = range[0]; i <= range[1]; i++) {
      if (i < 0) std::cout << i << std::endl;
      if (sequenceRepeatedAtLeastTwice(i)) sum += i;
    }
  }
  std::cout << sum << std::endl;
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

bool sequenceRepeatedTwice(unsigned long long number) {
  std::string fullString = std::to_string(number);
  int fullLength = fullString.length();
  if(fullLength % 2 != 0) return false;
  
  int half = fullLength/2;
  std::string firstHalf = fullString.substr(0, half);
  std::string lastHalf  = fullString.substr(half);
  return firstHalf == lastHalf;
}

bool sequenceRepeatedAtLeastTwice(unsigned long long number) {
  std::string fullString= std::to_string(number);
  for (int i = 0; i < fullString.length(); i++) {
    if (fullString.substr(0, i) == fullString.substr(i, i)) {
      return true;
    }
  }
  return false;
}


