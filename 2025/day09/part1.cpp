#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct node {
  int x;
  int y;
};

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


int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Couldn't open file" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<node> redTiles;
  while (std::getline(file, line)) {
    std::vector<std::string> stringCoords = split(line, ",");
    node temp;
    temp.x = std::stoi(stringCoords[0]);
    temp.y = std::stoi(stringCoords[1]);
    redTiles.push_back(temp);
  }

  long long largestArea = 0;
  for (int a = 0; a < (int) redTiles.size() - 1; a++) {
    for (int b = a + 1; b < (int)(int)  redTiles.size(); b++) {
      int width = abs(redTiles[a].x - redTiles[b].x) + 1;
      int height = abs(redTiles[a].y - redTiles[b].y) + 1;

      long long newArea = (long long) width * height;
      largestArea = largestArea < newArea? newArea : largestArea; 
    }
  }

  std::cout << largestArea << std::endl;
  return 0;
}
