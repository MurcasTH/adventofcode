#include <cstddef>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

struct node {
  int x;
  int y;
  int z;
};

struct Edge {
  int a;
  int b;
  long double distance;
};

struct DSU {
  std::vector<int> parent, rank;
  int components;

  explicit DSU(int n) : parent(n), rank(n,0), components(n) {
    for (int i = 0; i < n; i++) parent[i] = i;
  }

  int find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);

    if (a == b) return false;
    if (rank[a] < rank[b]) std::swap(a, b);
    parent[b] = a;
    if (rank[a] == rank[b]) rank[a]++;

    components--;
    return true;
  }
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

double vectorDistance(node vec1, node vec2) {
  long long xpos = vec1.x - vec2.x;
  long long ypos = vec1.y - vec2.y;
  long long zpos = vec1.z - vec2.z;
  long double sum = (long double) xpos*xpos + (long double)ypos*ypos + (long double)zpos*zpos;
  return std::sqrt((long double) sum);
}

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Couldn't open file" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<node> junctionBoxList;
  while (std::getline(file, line)) {
    std::vector<std::string> stringCoords = split(line, ",");
    node temp;
    temp.x = std::stoi(stringCoords[0]);
    temp.y = std::stoi(stringCoords[1]);
    temp.z = std::stoi(stringCoords[2]);
    junctionBoxList.push_back(temp);
  }

  const int N = (int) junctionBoxList.size();
  if (N < 2) {
    std::cerr << "Need at least 2 nodes" << std::endl;
    return 1;
  }

  std::vector<Edge> WorkingSet;
  WorkingSet.reserve((size_t)N * (N - 1) / 2);

  for (int a = 0; a < N; ++a) {
    for (int b = a + 1; b < N; ++b) {
      long double dist = vectorDistance(junctionBoxList[a], junctionBoxList[b]);
      WorkingSet.push_back({a, b, dist});
    }
  }

  std::sort(WorkingSet.begin(), WorkingSet.end(), [](const Edge& x, const Edge& y) {return x.distance < y.distance;});

  std::cout << "Using shortest edges: " << WorkingSet.size() << std::endl;

  DSU dsu(N);
  long double lastEdgeUsed = 0;
  int merges = 0;

  for (const auto& e : WorkingSet) {
    if (dsu.unite(e.a, e.b)) {
      merges++;
      lastEdgeUsed = e.distance;
      if (dsu.components == 1) {
        std::cout << "xPos product of last two nodes:" << junctionBoxList[e.a].x * junctionBoxList[e.b].x << std::endl;
        break;
      }
    }
  }

  return 0;
}



