#pragma once
#include "TriangleGrid.h"
#include "ColorController.h"
#include <map>
#include <QImage>
#include <QSharedPointer>
#include <QtConcurrent/QtConcurrent>
//#include <QtConcurrent>
#include <algorithm>
#include <utility>

typedef struct
{
  int x, y;
}Point;

class Polygon {
 protected:
  std::vector<Point> points;
 public:
  virtual double getArea() = 0;  // Pure virtual function
  std::vector<Point> getPoints() const;
};


class PixelsTriangle : public Polygon{
 public:
  PixelsTriangle() = default;
  PixelsTriangle(Point a, Point b, Point c);
  double getArea() override;
  QVector<PixelsTriangle> toVector() const {
    QVector<PixelsTriangle> pixelTriangles;
    for (size_t i = 0; i < points.size(); i += 3) {
      pixelTriangles.append(
          PixelsTriangle(points[i], points[i + 1], points[i + 2]));
    }
    return pixelTriangles;
  }
};

class TriangleController {
 private:
  int width, height;
  int num_cols, num_rows;
  int columnStep, rowStep;
  std::vector<Triangle> triangles;
  std::vector<PixelsTriangle> pixelsTriangles;
  Vector3D lightPosition;
  Vector3D** normalizedNormalvectors;
  double** Zs;
  bool** alreadySetted;
  void createPixelsTriangles(int c, int r);
  void getNewNUandZpdated(int x, int y, std::vector<Point>& points,
                              int triangleArea, Triangle t);
  void fillNormalizedVectorsAndZTable();
  QSharedPointer<QImage> normalMapImage;
 public:
  std::vector<PixelsTriangle> getPixelsTriangles();
  TriangleController(int w, int h, const std::vector<Triangle>& tris, int c,
                     int r);
  TriangleController() = default;
  void setNormalMapImage(QSharedPointer<QImage> nmapIm);
  void FillTriangles(QSharedPointer<QImage> image, Vector3D lightpos, bool isIm,
                     QSharedPointer<QImage> uplIm, bool isNMap);
};

typedef struct {
  Point start, end;
} Edge;

typedef struct ETNode {
  int y_max, x_min;
  double m;
  Edge edge;
} ETNode;

class ScanLine {
 private:
  int numOfAddedEdges = 0;
  double epsilon = std::numeric_limits<double>::epsilon(); 
  std::vector<Point> points;
  Triangle triangle;
  PixelsTriangle pixelsTriangle;
  double triangleArea;
  int start;
  int length, width, height;
  std::vector<ETNode>** bucketSortedEdgeTable;
  Edge* edges;
  void createEdges();
  void createET();
  void FillBetweenXs(int y, int x1, int x2, Vector3D lightPos, bool isIm);
  ColorController colorController;
  QSharedPointer<QImage> image;
  QSharedPointer<QImage> uploadedImage;
  QSharedPointer<QImage> normalMapImage;
  bool isNMap;
  Vector3D** normalizedNormalvectors;
  double** Zs;
 public:
  ScanLine(int width, int height, Vector3D** arr, double** zs,
           QSharedPointer<QImage> image, QSharedPointer<QImage> uplIm,
           bool isNmap, QSharedPointer<QImage> nMapImage);
  void configureScanLine(PixelsTriangle p, int st, int en, Triangle t);
  void Fill(Vector3D lightpos, bool isIm);
};
