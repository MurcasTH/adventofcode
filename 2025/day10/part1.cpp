#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct machinery {
  std::string diagram;
  std::vector<std::vector<int>> buttons;
  std::vector<int> joltages;
};


//function to split a string by a delimiter to a vector array
std::vector<std::string> split(std::string s, const std::string &del) {
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

std::vector<int> encapSToIVector(const std::string &s, std::string del) {
  std::string noEncap = s.substr(1,s.size()-2);
  //std::cout << noEncap << std::endl;
  std::vector<std::string> splitString = split(noEncap, del);
  std::vector<int> intVector;
  for (std::string &sNum : splitString) {
    //std::cout << sNum << std::endl;
    intVector.push_back(std::stoi(sNum));
  }
  return intVector;
}

int count1s(int num) {
  int count = 0;
  while (num) {
    count += (num & 1);
    num >>= 1;
  }
  return count;
}

unsigned long long translateToBinary(const std::string &repBin, const char zero, const char one) {
  unsigned long long binary = 0;
  for (size_t i = repBin.size(); i-- > 0;) {
    if (repBin[i] == one) {
      binary |= (1ULL << i);
    }
  }
  return binary;
}


int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Couldn't open file" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<machinery> factory;
  while (std::getline(file, line)) {
    std::vector<std::string> information = split(line, " ");
    machinery temp;
    temp.diagram = information[0].substr(1,information[0].size()-2);
    temp.joltages = encapSToIVector(information[information.size()-1], ",");
    
    for (int c = 1; c < (int) information.size()-1; c++) {
      temp.buttons.push_back(encapSToIVector(information[c], ","));
    }

    factory.push_back(temp);
  }


  //TODO: Find least button presses necessary to get diagram
  
  int sum = 0;
  for (const machinery &machine : factory) {
    int buttonAmount = machine.buttons.size();
    int leastButtonPresses = buttonAmount;
    for (int combi = 0; combi < (1 << buttonAmount); combi++) {
      int result = 0;
      for (int buttonIndex = 0; buttonIndex < buttonAmount; buttonIndex++) {
        if((combi >> buttonIndex) & 1) {
          for(const int& index : machine.buttons[buttonIndex]) {
            result ^= (1 << index);
            //TODO: remember to flip result in comparison with diagram
          }
        }
      }
      std::cout << result << " = " << translateToBinary(machine.diagram, '.', '#') << std::endl;
      if ((unsigned long) result == translateToBinary(machine.diagram, '.', '#')) {
        int buttonPresses = count1s(combi);
        leastButtonPresses = (leastButtonPresses < buttonPresses)? leastButtonPresses : buttonPresses;
      }
    }
    sum += leastButtonPresses;
  }

  std::cout << sum << std::endl;


  // for (const machinery &id : factory) {
  //   std::cout << "Diagram: " << id.diagram << ", Buttons: ";
  //   for (const std::vector<int> &button : id.buttons) {
  //     std::cout << "(";
  //     for (int c = 0; c < (int) button.size(); c++) {
  //       std::cout << button[c];
  //       if(c != (int) button.size() - 1) std::cout << ",";
  //     }
  //     std::cout << ") ";
  //   }
  //   std::cout << "Joltages: {";
  //   for (const int &jolt : id.joltages) {
  //     std::cout << jolt << ",";
  //   }
  //   std::cout << "}" << std::endl;
  // }

  return 0;
}
