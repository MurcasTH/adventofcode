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
cpp_int countPaths(int r, int c, std::vector<std::string> &map, std::vector<std::vector<cpp_int>> &memo);

static const cpp_int NoEntry = -1;

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


  std::vector<std::vector<cpp_int>> memo(mapHeight, std::vector<cpp_int>(mapWidth, NoEntry));
  //laserTraversal(0, startCol, map, answer);

  cpp_int answer = countPaths(0, startCol, map, memo);
  
  std::cout << mapWidth << " : " << mapHeight << std::endl;
  std::cout << answer << std::endl;
  return 0;
}


cpp_int countPaths(int r, int c, std::vector<std::string> &map, std::vector<std::vector<cpp_int>> &memo) {
  const int mapHeight = map.size();
  const int mapWidth = map[0].size();

  if (c < 0 || c >= mapWidth) return 0;
  if (r == mapHeight - 1) return 1;
  if (memo[r][c] != NoEntry) return memo[r][c];

  char below = map[r+1][c];
  cpp_int res = 0;
  switch (below) {
    case EMPTY:
      res = countPaths(r+1, c, map, memo);
      break;
    case SPLITTER:
      res = countPaths(r+1, c+1, map, memo) + countPaths(r+1, c-1, map, memo);
    break;
    default:
      res = 0;
    break;
  }
  memo[r][c] = res;
  return res;
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

