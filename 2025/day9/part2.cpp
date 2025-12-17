#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct point {
  int x;
  int y;
};

struct edge {
  point a;
  point b;
};

bool pointOnEdge(const point &p, const edge &e) {
  long long cross = (long long) (e.b.x - e.a.x) * (p.y - e.a.y) - (long long) (e.b.y - e.a.y) * (p.x - e.a.x);
  
  if (cross != 0 ) return false;
  return p.x >= std::min(e.a.x, e.b.x) && 
         p.x <= std::max(e.a.x, e.b.x) &&
         p.y >= std::min(e.a.y, e.b.y) &&
         p.y <= std::max(e.a.y, e.b.y);
}

bool pointInPolyOrOnBoundry(const point &p, const std::vector<edge> &poly) {
  for (const auto& e : poly) if (pointOnEdge(p, e)) return true;

  bool inside = false;
  for (const auto& e : poly) {
    // consider only vertical edges for crossings
    if (e.a.x == e.b.x) {
      int x = e.a.x;
      int y1 = e.a.y, y2 = e.b.y;
      int ymin = std::min(y1,y2), ymax = std::max(y1,y2);

      // half-open: [ymin, ymax)
      if (p.y >= ymin && p.y < ymax) {
        if (x > p.x) inside = !inside;
      }
    }
  }
  return inside;
}

bool isHorizontal(const edge &e){return e.a.y == e.b.y;}
bool isVertical(const edge &e){return e.a.x == e.b.x;}

bool crossAxisAligned(const edge &vertical, const edge &horizontal) {
  int VerticalX = vertical.a.x;
  int HorizontalY = horizontal.a.y;
  
  int VerticalMinY = std::min(vertical.a.y, vertical.b.y), VerticalMaxY = std::max(vertical.a.y, vertical.b.y);
  int HorizontalMinX = std::min(horizontal.a.x, horizontal.b.x), HorizontalMaxX = std::max(horizontal.a.x, horizontal.b.x);

  return (HorizontalMinX < VerticalX && VerticalX < HorizontalMaxX) && (VerticalMinY < HorizontalY && HorizontalY < VerticalMaxY);
}

bool rectangleCrossesPolygon(const std::vector<edge> &rect, const std::vector<edge> poly) {
  for (const auto& re : rect) {
    for (const auto& pe : poly) {
      if (isVertical(re) && isHorizontal(pe) && crossAxisAligned(re, pe)) return true;
      if (isHorizontal(re) && isVertical(pe) && crossAxisAligned(pe, re)) return true;
    }
  }
  return false;
}


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


// I know point AA and point BB is inside my polygon
// I need to know if any edge of my triangle, instersects any edge on my polygon.

bool edgeVsEdge(edge edge1, edge edge2) {
  int dx1 = edge1.b.x - edge1.a.x;
  int dy1 = edge1.b.y - edge1.a.y;
  int dx2 = edge2.b.x - edge2.a.x;
  int dy2 = edge2.b.y - edge2.a.y;
  int dx3 = edge1.a.x - edge2.a.x;
  int dy3 = edge1.a.y - edge2.a.y;

  int d = dx1*dy2 - dy1*dx2;
  if(d == 0) {
    return false;
  } 
  int t1 = (dx2*dy3 - dy2*dx3)/d;
  if (t1 < 0 || t1 > 1) {
    return false;
  }

  int t2 = (dx1*dy3 - dy1*dx3)/d;
  if (t2 < 0 || t2 > 1) {
    return false;
  }
  return true;
}

int main (int argc, char *argv[]) {
  std::fstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Couldn't open file" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<point> redTiles;
  while (std::getline(file, line)) {
    std::vector<std::string> stringCoords = split(line, ",");
    point temp;
    temp.x = std::stoi(stringCoords[0]);
    temp.y = std::stoi(stringCoords[1]);
    redTiles.push_back(temp);
  }

  std::vector<edge> polygon;
  for (int c = 0; c < (int) redTiles.size() - 1; c++) {
    edge temp;
    temp.a = redTiles[c];
    temp.b = redTiles[c+1];
    polygon.push_back(temp);
  }
  polygon.push_back({redTiles[redTiles.size() -1], redTiles[0]});


  //Create a rectangle by defining 2 points from redTiles vector
  std::vector<std::vector<edge>> rectangles;
  for (int x = 0; x < (int) redTiles.size() - 1; x++) {
    for (int y = x + 1; y < (int) redTiles.size(); y++) {
      std::vector<edge> rectangle;
      rectangle.push_back({redTiles[x], {redTiles[x].x, redTiles[y].y}});
      rectangle.push_back({{redTiles[x].x, redTiles[y].y}, redTiles[y]});
      rectangle.push_back({redTiles[y], {redTiles[y].x, redTiles[x].y}});
      rectangle.push_back({{redTiles[y].x, redTiles[x].y}, redTiles[x]});
      rectangles.push_back(rectangle);
    }
  }
  
  long long largestArea = 0;
  for (int r = 0; r < (int) rectangles.size(); r++) {
    bool isValid = true;
    point A = rectangles[r][0].a;
    point B = rectangles[r][1].b;
    point C = rectangles[r][0].b;
    point D = rectangles[r][2].b;

    if (!pointInPolyOrOnBoundry(A, polygon) ||
        !pointInPolyOrOnBoundry(B, polygon) ||
        !pointInPolyOrOnBoundry(C, polygon) ||
        !pointInPolyOrOnBoundry(D, polygon)) {
      isValid = false;
    }

    // auto mid = [](point &a, point &b) -> point{
    //   return {(a.x+b.x)/2, (a.y+b.y)/2};
    // };
    //
    // point mAC = mid(A, C);
    // point mCB = mid(C, B);
    // point mBD = mid(B, D);
    // point mDA = mid(D, A);
    //
    // if (!pointInPolyOrOnBoundry(mAC, polygon) ||
    //     !pointInPolyOrOnBoundry(mCB, polygon) ||
    //     !pointInPolyOrOnBoundry(mBD, polygon) ||
    //     !pointInPolyOrOnBoundry(mDA, polygon)) {
    //   isValid = false;
    // }

    if(rectangleCrossesPolygon(rectangles[r], polygon)) isValid = false;

    if (isValid) {
      int width = abs(rectangles[r][0].b.x - rectangles[r][rectangles[r].size()-1].a.x) + 1;
      int height = abs(rectangles[r][0].b.y - rectangles[r][rectangles[r].size()-1].a.y) + 1;
      long long newArea = (long long) width*height;
      largestArea = (largestArea < newArea)? newArea : largestArea;
    }
  }
  std::cout << rectangles.size() << " : " << polygon.size() << std::endl;
  std::cout << rectangles.size() * polygon.size() << std::endl;

  std::cout << largestArea << std::endl;
  return 0;
}
