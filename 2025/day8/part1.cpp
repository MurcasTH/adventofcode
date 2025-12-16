#include <cmath>
#include <cstddef>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
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

  explicit DSU(int n) : parent(n), rank(n,0) {
    for (int i = 0; i < n; i++) parent[i] = i;
  }

  int find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
  }

  void unite(int a, int b) {
    a = find(a);
    b = find(b);

    if (a == b) return;
    if (rank[a] < rank[b]) std::swap(a, b);
    parent[b] = a;
    if (rank[a] == rank[b]) rank[a]++;
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

  const int K = 1000;
  auto cmp = [](const Edge& a, const Edge& b) {return a.distance < b.distance;};
  std::priority_queue<Edge, std::vector<Edge>, decltype(cmp)> heap(cmp);

  for (int a = 0; a < N; ++a) {
    for (int b = a + 1; b < N; ++b) {
      long double dist = vectorDistance(junctionBoxList[a], junctionBoxList[b]);
      Edge e{a, b, dist};

      if((int)heap.size() < K) {
        heap.push(e);
      } else if (dist < heap.top().distance) {
        heap.pop();
        heap.push(e);
      }
    }
  }

  std::vector<Edge> WorkingSet;
  WorkingSet.reserve(heap.size());
  while (!heap.empty()) {
    WorkingSet.push_back(heap.top());
    heap.pop();
  }
  std::sort(WorkingSet.begin(), WorkingSet.end(), [](const Edge& x, const Edge& y) {return x.distance < y.distance;});

  std::cout << "Using shortest edges: " << WorkingSet.size() << std::endl;

  DSU dsu(N);
  for (const auto& e : WorkingSet) {
    dsu.unite(e.a, e.b);
  }

  std::unordered_map<int, std::vector<int>> groups;
  groups.reserve((size_t) N);

  for(int i = 0; i < N; ++i) {
    groups[dsu.find(i)].push_back(i);
  }

  std::vector<std::vector<int>> networks;
  networks.reserve(groups.size());
  for(auto& [root, members] : groups) {
    networks.push_back(std::move(members));
  }

  std::sort(networks.begin(), networks.end(), [](const auto& a, const auto& b) {return a.size() > b.size();});

  std::cout << "Networks found: " << networks.size() << "\n";
  for (size_t i = 0; i < networks.size(); ++i) {
    std::cout << "Network: " << i << " size: " << networks[i].size() << std::endl;
  }

  if (networks.size() >= 3) {
    long long prod = 1;
    prod *= (long long) networks[0].size();
    prod *= (long long) networks[1].size();
    prod *= (long long) networks[2].size();
    std::cout << "Top-3 product: " << prod << std::endl;
  }


  return 0;
}



