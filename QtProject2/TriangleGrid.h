#pragma once
class BezierController;
#include "BezierController.h"
#include<cmath>
#include<limits>

class TriangleVertex
{
 public:
  double x, y, z;
  Vector3D N, Nmapped;
  TriangleVertex() = default;
  TriangleVertex(const TriangleVertex& other)
      : x(other.x), y(other.y), z(other.z), N(other.N), Nmapped(other.Nmapped) {}
  TriangleVertex& operator=(const TriangleVertex& other);
};


class Triangle {
 private:
  double epsilon = std::numeric_limits<double>::epsilon(); 
 public:
  int Id;
  TriangleVertex A, B, C;
  Triangle() = default;
  Triangle(TriangleVertex a, TriangleVertex b, TriangleVertex c, int i);
  TriangleVertex getA() { return A; }
  TriangleVertex getB() { return B; }
  TriangleVertex getC() { return C; }
  bool oneOfVertecesXY(double x, double y);
};


class TriangleGrid {
 private:
  int cols, rows;
  /*Point** points;
  double** zCoordinates;
  Vector3D** normalVectors;*/
  BezierController* bezierController;
  TriangleVertex** triangleVerteces;
  std::vector<Triangle> triangles;
 public:
  TriangleGrid() = default;
  TriangleGrid(BezierController& b, int c = 6, int r = 4 );
  std::vector<Triangle>& getTriangles();
  ~TriangleGrid();
};
