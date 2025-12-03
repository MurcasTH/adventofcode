#include <fstream>
#include <string>
#include <iostream>

struct digitInString {
  int value = 0;
  int pos = 0;
};

int findLargestNumber(std::string s) {
  int len = s.length();
  digitInString first, last;
  for (int i = 0; i < len - 1; i++) {
    int digitTest = s[i] - '0';
    if (digitTest > first.value) {
      first.value = digitTest;
      first.pos = i;
    }
  }

  for (int i = first.pos + 1; i < len; i++) {
    int digitTest = s[i] - '0';
    if (digitTest > last.value) {
      last.value = digitTest;
      last.pos = i;
    }
  }
  int largestNumber = first.value * 10 + last.value;
  //std::cout << s << ": " << largestNumber << std::endl;
  
  return largestNumber;
}

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);

  
  std::string line;
  int sum = 0;
  while (std::getline(file, line)){
    sum += findLargestNumber(line);
  }
  std::cout << "sum: " <<sum << std::endl;
  return 0;
}
