#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define DIALMINVAL 0
#define DIALMAXVAL 99

void dial(int* dialValue, int rotationValue);

struct dialInstruction {
  char direction;
  int value;
};
int dialValue = 50;
int zerosCount = 0;

int main (int argc, char *argv[]) {
  std::fstream file("input");
  if (!file.is_open()) {
    std::cerr << "Failed to open file\n";
    return 1;
  }

  std::string line;
  std::vector<dialInstruction> dialInstructions;
  while (std::getline(file, line)) {
    dialInstruction temp;
    temp.direction = line[0];
    temp.value = std::stoi(line.substr(1, line.length() - 1));
    dialInstructions.push_back(temp);
  }
 
  for(dialInstruction instruction : dialInstructions) {
    switch (instruction.direction) {
      case 'R':
        dial(&dialValue, instruction.value);
        break;
      case 'L':
        dial(&dialValue, -instruction.value);
        break;
      default:
        std::cerr << "instruction not viable\n";
    }
    std::cout << dialValue << ": " << instruction.direction << instruction.value << std::endl;
  }

  std::cout << zerosCount << std::endl;

  return 0;
}

void dial(int* dialValue, int rotationValue) {
  if (rotationValue < DIALMINVAL) {
    for (int i = rotationValue; i < DIALMINVAL; i++) {
      *dialValue -= 1;
      if (*dialValue < 0) *dialValue = DIALMAXVAL;
      if (*dialValue == 0) zerosCount++;
    }
  } else {
    for (int i = DIALMINVAL; i < rotationValue; i++) {
      *dialValue += 1;
      if (*dialValue > DIALMAXVAL) *dialValue = DIALMINVAL;
      if (*dialValue == 0) zerosCount++;
    }
  }
}
