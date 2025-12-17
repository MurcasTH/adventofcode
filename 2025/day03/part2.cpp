#include <array>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>

struct digitInString {
  int value = 0;
  int pos = 0;
};

digitInString findLargestDigit(std::string s, int startPos, int digitPlacement) {
  digitInString temp;
  int len = s.length();
  for (int i = startPos; i < len - digitPlacement + 1; i++) {
    int digitTest = s[i] - '0';
    if (digitTest > temp.value) {
      temp.value = digitTest;
      temp.pos = i;
    }
  }
  return temp;
}

unsigned long long findLargestNumber(std::string s) {
  std::array<digitInString, 12> arr;
  arr[0] = findLargestDigit(s, 0, 12);
  //std::cout << s << ": " << arr[0].value << ", ";
  for (int i = 1; i < 12; i++) {
    arr[i] = findLargestDigit(s, arr[i-1].pos + 1, 12 - i);
    //std::cout << arr[i].value << ", ";
  }
  

  unsigned long long number = 0;
  for (int i = 0; i < 12; i++) {
    number += arr[i].value * std::pow(10, (12 - i - 1));
  }
  //std::cout << number << std::endl;
  return number;
}

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);

  std::string line;
  unsigned long long sum = 0;
  while (std::getline(file, line)){
    sum += findLargestNumber(line);
  }
  std::cout << "sum: " <<sum << std::endl;
  return 0;
}
