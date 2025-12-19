#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string_view>

const char delimiter = ' ';

struct node {
  long long pathsToEndpoint = -1;
  std::string name;
  std::vector<node*> children;
  std::vector<node*> parents;
};

using nodeMap = std::unordered_map<std::string, std::unique_ptr<node>>;

void findEndPoints(node &point, const std::string_view endpoint) {
  for(node *child : point.children) {
    if(child->name == endpoint) {
      point.pathsToEndpoint++;
      return;
    }
  }
}

static inline void forEachToken(std::string_view tokenText, const std::function<void(std::string_view)>& lambdaFunction) {
  int start_index = 0;
  int tokenTextSize = tokenText.size();

  while(start_index < tokenTextSize) {
    while(start_index < tokenTextSize && tokenText[start_index] == delimiter) start_index++;
    if(start_index >= tokenTextSize) break;

    int end_index = start_index;
    while(end_index < tokenTextSize && tokenText[end_index] != delimiter) end_index++;

    lambdaFunction(tokenText.substr(start_index, end_index - start_index));
    start_index = end_index;
  }
}

static node* getOrCreate(nodeMap& nodes, std::string_view name) {
  nodeMap::iterator it = nodes.find(std::string(name));
  if (it != nodes.end()) return it->second.get();

  auto [it2, wasInserted] = nodes.emplace(name, std::make_unique<node>());
  return it2->second.get();
}



static void parseLineAndLink(nodeMap& nodes, std::string_view line) {
  const size_t colonPos = line.find(':');
  if (colonPos == std::string_view::npos) return;
  
  std::string_view parentName = line.substr(0, colonPos);
  
  size_t childNamesPos = colonPos+1;
  while (childNamesPos < line.size() && line[childNamesPos] == delimiter) childNamesPos++;

  std::string_view childNames = line.substr(childNamesPos);

  node* parent = getOrCreate(nodes, parentName);
  parent->name = parentName;
  //std::cout << "Parent: " << parentName << std::endl;
  forEachToken(childNames, [&](std::string_view childName) {
    node* child = getOrCreate(nodes, childName);
    parent->children.push_back(child);
    child->parents.push_back(parent);
    child->name = childName;
    //std::cout << "Child: " << std::string(childName) << std::endl;
  });
}

static long long calculateEndPaths(nodeMap& nodes, std::string_view nodeName, std::string_view endpoint) {
  if (nodeName == std::string(endpoint)) return 1ULL;

  nodeMap::iterator node_iterator = nodes.find(std::string(nodeName));
  if (node_iterator == nodes.end() || node_iterator->second.get() == nullptr) return 0;

  std::unique_ptr<node>& currentNode = nodes[std::string(nodeName)];
  if (currentNode->pathsToEndpoint != -1) return currentNode->pathsToEndpoint;
  if (currentNode->children.empty()) {
    currentNode->pathsToEndpoint = 0ULL;
    return 0ULL;
  }

  long long sum_paths = 0ULL;
  for (node* child : currentNode->children) {

    sum_paths += calculateEndPaths(nodes, child->name, endpoint);
  }
  currentNode->pathsToEndpoint = sum_paths;
  return sum_paths; 
}

static int reCalculateEndPaths(nodeMap& nodes, std::string_view nodeName, std::string_view endpoint) {
  for (std::pair<const std::string, std::unique_ptr<node>>& nodesEntry : nodes) {
    nodesEntry.second->pathsToEndpoint = -1;
  }

  return calculateEndPaths(nodes, nodeName, endpoint);
}

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Couldn't open file" << std::endl;
    return -1;
  }
  
  nodeMap network;
  std::string line;
  while (std::getline(file, line)) {
    parseLineAndLink(network, line);
  }

  int svr2fft = calculateEndPaths(network, "svr", "fft");
  int fft2dac = reCalculateEndPaths(network, "fft", "dac");
  int dac2out = reCalculateEndPaths(network, "dac", "out");

  int svr2dac = reCalculateEndPaths(network, "svr", "dac");
  int dac2fft = reCalculateEndPaths(network, "dac", "fft");
  int fft2out = reCalculateEndPaths(network, "fft", "out");

  long long svr2fft2dac2out = (long long) svr2fft * fft2dac * dac2out;
  long long svr2dac2fft2out = (long long) svr2dac * dac2fft * fft2out;

  long long totalPaths = svr2fft2dac2out + svr2dac2fft2out;

  
  std::cout << "Paths: " << totalPaths << std::endl;

  return 0;
}

float decimal = 0.123;
int heltall = 10;
char symbol = ' ';
bool trueOrFalse = true;

int add(int a, int b) {
  return a+b;
}
