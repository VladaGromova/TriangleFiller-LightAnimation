#include "TriangleGrid.h"

TriangleGrid::TriangleGrid(BezierController& b, int c, int r)
    : bezierController(&b) {
  //*bezierController = b;
  cols = c;
  rows = r;
  triangleVerteces = new TriangleVertex*[rows + 1];
  for (int i = 0; i <= rows; ++i) {
    triangleVerteces[i] = new TriangleVertex[cols + 1];
  }
  double columnStep = 1.0 / cols;
  double rowStep = 1.0 / rows;
  double x, y;
  for (int i = 0; i <= rows; ++i) {
    for (int j = 0; j <= cols; ++j) {
      x = j * columnStep;
      y = i * rowStep;
      triangleVerteces[i][j].x = x;
      triangleVerteces[i][j].y = y;
      triangleVerteces[i][j].z = bezierController->CalculateZ_Bezier(x, y);
      triangleVerteces[i][j].N =
          bezierController->getNormalizedNormalVector(x, y);
    }
  }

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      triangles.push_back(Triangle(triangleVerteces[i][j],
                                   triangleVerteces[i+1][j],
                                   triangleVerteces[i + 1][j+1],i*rows +j));
      triangles.push_back(Triangle(triangleVerteces[i+1][j+1],
                                   triangleVerteces[i][j+1],
                                   triangleVerteces[i][j], i * rows + j + 1));
    }
  }

}

TriangleGrid ::~TriangleGrid() {
  
  for (int i = 0; i <= rows; ++i) {
    delete[] triangleVerteces[i];
  }
  delete[] triangleVerteces;
}

std::vector<Triangle>& TriangleGrid::getTriangles() { return triangles; }

Triangle::Triangle(TriangleVertex a, TriangleVertex b, TriangleVertex c, int i)
    : A(a), B(b), C(c) {
  Id = i;
}

bool Triangle::oneOfVertecesXY(double x, double y) {
  if ((std::fabs(A.x - x) < epsilon) && (std::fabs(A.y - y) < epsilon)) {
    return true;
  }
  if ((std::fabs(B.x - x) < epsilon) && (std::fabs(B.y - y) < epsilon)) {
    return true;
  }
  if ((std::fabs(C.x - x) < epsilon) && (std::fabs(C.y - y) < epsilon)) {
    return true;
  }
  return false;
}


TriangleVertex& TriangleVertex::operator=(const TriangleVertex& other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    N = other.N;
  }
  return *this;
}
