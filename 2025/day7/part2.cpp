#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

#define LASER  '|'
#define EMPTY  '.'
#define SPLITTER  '^'
#define EMITTER  'S'

void laserTraversal(int r, int c, std::vector<std::string> &map, cpp_int &paths);

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cout << "Could not open file" << std::endl;
    return 1;
  }


 
  std::vector<std::string> map;
  std::string line;
  while (std::getline(file, line)) {
    map.push_back(line);
  }
 
  const int mapHeight = map.size();
  const int mapWidth = map[0].size();

  int startCol = -1; //cause error incase S is not found
  for (int c = 0; c < mapWidth; c++) {
    if (map[0][c] == EMITTER) {
      startCol = c;
      break;
    }
  }

  if (startCol == -1) {
    std::cerr << "No emitter 'S' found on top row" << std::endl;
    return 1;
  }

  cpp_int answer = 0;

  laserTraversal(0, startCol, map, answer);
  
  std::cout << answer << std::endl;
  return 0;
}

void laserTraversal(int r, int c, std::vector<std::string> &map, cpp_int &paths) {
  const int mapHeight = map.size();
  const int mapWidth = map[0].size();
  if (r == mapHeight - 1) {
    paths++; // If path reached the end, add it to possible paths
    return;
  }
  char below = map[r+1][c];
  switch (below) {
    case EMPTY:
      laserTraversal(r+1, c, map, paths);
      break;
    case SPLITTER:
      if(c + 1 < mapWidth) laserTraversal(r+1, c+1, map, paths);
      if(c - 1 >= 0) laserTraversal(r+1, c-1, map, paths);
    break;
  }
  return;
}

