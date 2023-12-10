#pragma once
#include<math.h>
#include<cstdlib>
#include <time.h>
#include <random>

class Vector3D {
 private:
  double x, y, z;

 public:
  Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
  Vector3D() : x(0.0), y(0.0), z(0.0) {}
  Vector3D(const Vector3D& other) : x(other.x), y(other.y), z(other.z){}
  double X();
  double Y();
  double Z();
  void setX(double X) { x = X; }
  void setY(double Y) { y = Y; }
  void setZ(double Z) { z = Z; }
  double getLength() const { return sqrt(x * x + y * y + z * z); }
  Vector3D getNormalized() const;
  Vector3D scalarMultiply(double scalar) const;
  Vector3D subtract(const Vector3D& other) const;
  double dotProduct(Vector3D b);
  double angleBetween(const Vector3D& L);
  static Vector3D crossProduct(Vector3D& a, Vector3D& b);
  Vector3D& operator=(const Vector3D& other);
  Vector3D normalVectorWithNormalMap(Vector3D N_Bezier_normalized,
                                     Vector3D N_norm_map);
};

class BezierController {
 private:
  int cols = 4, rows = 4;
  Vector3D** main_vectors;
  double z_i_j[4][4] = {{0.0, 0.5, 0.5, 0},
	                    {0.5, 1.0, 1.0, 0.5},
                        {0.5, 1.0, 1.0, 0.5},
                        {0.0, 0.5, 0.5, 0}};
  //double z_i_j[4][4] = {{0.0, 0.5, 0.5, 0},
  //                      {0.5, 0.2, 0.2, 0.5},
  //                      {0.5, 0.2, 0.2, 0.5},
  //                      {0.0, 0.5, 0.5, 0}};
  //double** z_Bezier;
  //void initZ(); // TO DO: not random
  Vector3D calculatePx(double x, double y);
  Vector3D calculatePy(double x, double y);
 public:
  BezierController(int _n = 4, int _m = 4); // control x, y, z_i_j
  ~BezierController();
  BezierController(const BezierController& other);
  double CalculateZ_Bezier(double x, double y); // na wejsciu dowolone x, y z <0,1>
  Vector3D getNormalVector(double x, double y);
  Vector3D getNormalizedNormalVector(double x, double y);
  Vector3D** getMainVectors();
};


