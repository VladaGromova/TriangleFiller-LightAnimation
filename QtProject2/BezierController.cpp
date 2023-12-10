#include "BezierController.h"

BezierController::BezierController(int _n, int _m) {
  cols = _n;
  rows = _m;
  main_vectors = new Vector3D*[rows];
  //initZ();
  for (int i = 0; i < rows; ++i) {
    main_vectors[i] = new Vector3D[cols];
    for (int j = 0; j < cols; ++j) {
      main_vectors[i][j] = Vector3D(j / (double)(cols - 1), i / (double)(rows - 1),
                                    z_i_j[i][j]);  // TO DO: change z 
    }
  }
}

double binomial(int n, int k) {
  std::vector<int> C(k + 1, 0);
  C[0] = 1;
  for (int i = 1; i <= n; i++) {
    for (int j = std::min(i, k); j > 0; j--) {
      C[j] = C[j] + C[j - 1];
    }
  }
  double res = (double)C[k];
  return res;
}


double B(int i, int n, double t) {
  return binomial(n, i) * pow(t, i) * pow(1 - t, n - i);
}

BezierController::~BezierController() {
  for (int i = 0; i < rows; ++i) {
    delete[] main_vectors[i];
  }
  delete[] main_vectors;
}

BezierController::BezierController(const BezierController& other)
    : cols(other.cols), rows(other.rows) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      z_i_j[i][j] = other.z_i_j[i][j];
    }
  }

  main_vectors = new Vector3D*[rows];
  for (int i = 0; i < rows; ++i) {
    main_vectors[i] = new Vector3D[cols];
    for (int j = 0; j < cols; ++j) {
      main_vectors[i][j] = other.main_vectors[i][j];
    }
  }
}

double BezierController::CalculateZ_Bezier(double x, double y) {
  double result = 0.0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      result += z_i_j[i][j] * B(i, rows - 1, x) * B(j, cols - 1, y);
    }
  }
  return result;
}

Vector3D BezierController::getNormalVector(double x, double y) {
  Vector3D a = calculatePx(x, y);
  Vector3D b = calculatePy(x, y);
  return Vector3D::crossProduct(a, b);
}

Vector3D BezierController::getNormalizedNormalVector(double x, double y) {
  return getNormalVector(x, y).getNormalized();
}

Vector3D BezierController::calculatePx(double x, double y) {
  double res = 0.0;
  for (int i = 0; i < rows - 1; ++i) {
    for (int j = 0; j < cols; ++j) {
      res += (z_i_j[i+1][j] - z_i_j[i][j]) * B(i, rows - 2, x) * B(j, cols - 1, y);
    }
  }
  return Vector3D(1, 0, 3*res);
}

Vector3D BezierController::calculatePy(double x, double y) {
  double res = 0.0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols-1; ++j) {
      res += (z_i_j[i][j+1] - z_i_j[i][j]) * B(i, rows - 1, x) *
             B(j, cols - 2, y);
    }
  }
  return Vector3D(0, 1, 3 * res);
}

Vector3D Vector3D::normalVectorWithNormalMap(Vector3D N_Bezier_normalized,
                                                   Vector3D N_norm_map) {
  Vector3D tmp = Vector3D(0, 0, 1);
  Vector3D B = Vector3D::crossProduct(N_Bezier_normalized, tmp);
  if (N_Bezier_normalized.X() == 0 && N_Bezier_normalized.Y() == 0 && N_Bezier_normalized.Z() == 1) {
    B = Vector3D(0, 1, 0);
  }
  Vector3D T = Vector3D::crossProduct(B, N_Bezier_normalized);
  Vector3D firstRow = Vector3D(T.X(), B.X(), N_Bezier_normalized.X());
  Vector3D secondRow = Vector3D(T.Y(), B.Y(), N_Bezier_normalized.Y());
  Vector3D thirdRow = Vector3D(T.Z(), B.Z(), N_Bezier_normalized.Z());
  double x_res = firstRow.dotProduct(N_norm_map);
  double y_res = secondRow.dotProduct(N_norm_map);
  double z_res = thirdRow.dotProduct(N_norm_map);
  return Vector3D(x_res, y_res, z_res);
}

Vector3D** BezierController::getMainVectors() { return main_vectors; }

double Vector3D::X() { return x; }
double Vector3D::Y() { return y; }
double Vector3D::Z() { return z; }

Vector3D Vector3D::getNormalized() const {
  double length = getLength();
  if (length != 0.0) {
    return Vector3D(x / length, y / length, z / length);
  } else {
    return Vector3D();
  }
}

Vector3D Vector3D::scalarMultiply(double scalar) const {
  return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::subtract(const Vector3D& other) const {
  return Vector3D(x - other.x, y - other.y, z - other.z);
}

double Vector3D::dotProduct(Vector3D b) {
  return x * b.X() + y * b.Y() + z * b.Z();
}

double Vector3D::angleBetween(const Vector3D& L) {
  double dotProduct = this->dotProduct(L);
  double magnitudeN = this->getLength();
  double magnitudeL = L.getLength();
  double cosine = dotProduct / (magnitudeN * magnitudeL);
  double angleRad = std::acos(cosine);
  return angleRad;
}

Vector3D Vector3D::crossProduct(Vector3D& a, Vector3D& b) {
  double x = a.Y() * b.Z() - a.Z() * b.Y();
  double y = a.Z() * b.X() - a.X() * b.Z();
  double z = a.X() * b.Y() - a.Y() * b.X();
  return Vector3D(x, y, z);
}

Vector3D& Vector3D::operator=(const Vector3D& other) {
  if (this != &other) {  // Avoid self-assignment
    x = other.x;
    y = other.y;
    z = other.z;
  }
  return *this;
}
