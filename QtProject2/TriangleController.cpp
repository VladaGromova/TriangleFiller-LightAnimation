#include "TriangleController.h"

auto isYMax = [](const ETNode& node, int y) { return node.y_max == y; };

double getSubArea(Point A, Point B, Point P) {
  double area = 0.5 * std::fabs(A.x * (B.y - P.y) + B.x * (P.y - A.y) +
                                P.x * (A.y - B.y));

  return area;
}

std::vector<PixelsTriangle> TriangleController::getPixelsTriangles() {
  return pixelsTriangles;
}

void TriangleController::createPixelsTriangles(int c, int r) {
  num_rows = r;
  num_cols = c;
  Point** pixVertArray = new Point*[r + 1];
  for (int i = 0; i <= r; ++i) {
    pixVertArray[i] = new Point[c + 1];
  }
  columnStep = width / c;
  rowStep = height / c;
  int x, y;
  for (int i = 0; i <= r; ++i) {
    for (int j = 0; j <= c; ++j) {
      x = j * columnStep;
      y = i * rowStep;
      pixVertArray[i][j].x = x;
      pixVertArray[i][j].y = y;
    }
  }

  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      pixelsTriangles.push_back(PixelsTriangle(pixVertArray[i][j],
                                               pixVertArray[i + 1][j],
                                               pixVertArray[i + 1][j + 1]));
      pixelsTriangles.push_back(PixelsTriangle(pixVertArray[i + 1][j + 1],
                                               pixVertArray[i][j + 1],
                                               pixVertArray[i][j]));
    }
  }
}

bool isPointInsideTriangle(const Point& p, const Point& a, const Point& b,
                           const Point& c) {
  double areaABC =
      0.5 * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
  double sign = areaABC < 0 ? -1.0 : 1.0;

  double s =
      (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y) * sign;
  double t =
      (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y) * sign;

  return s >= 0 && t >= 0 && (s + t) <= 2 * areaABC * sign;
}

std::vector<Point> getPointsInsideTriangle(const Point& a, const Point& b,
                                           const Point& c) {
  std::vector<Point> pointsInside;

  // Determine bounding box
  int minX = std::min({a.x, b.x, c.x});
  int maxX = std::max({a.x, b.x, c.x});
  int minY = std::min({a.y, b.y, c.y});
  int maxY = std::max({a.y, b.y, c.y});

  for (int x = minX; x <= maxX; ++x) {
    for (int y = minY; y <= maxY; ++y) {
      Point current = {x, y};
      if (isPointInsideTriangle(current, a, b, c)) {
        pointsInside.push_back(current);
      }
    }
  }

  return pointsInside;
}

void TriangleController::getNewNUandZpdated(int x, int y,
                                                std::vector<Point>& points,
                                                int triangleArea, Triangle t) {
  double alfa, beta, gamma;
  Point p;
  p.x = x;
  p.y = y;
  alfa = getSubArea(points[1], points[2], p) / triangleArea;
  beta = getSubArea(points[2], points[0], p) / triangleArea;
  gamma = getSubArea(points[0], points[1], p) / triangleArea;
  Vector3D modifiedA = t.getA().N.scalarMultiply(alfa);
  Vector3D modifiedB = t.getB().N.scalarMultiply(beta);
  Vector3D modifiedC = t.getC().N.scalarMultiply(gamma);
  Vector3D newN = Vector3D(modifiedA.X() + modifiedB.X() + modifiedC.X(),
                           modifiedA.Y() + modifiedB.Y() + modifiedC.Y(),
                           modifiedA.Z() + modifiedB.Z() + modifiedC.Z());
  normalizedNormalvectors[y][x] = newN;
  Zs[y][x] = t.getA().z * alfa + t.getB().z * beta +
             t.getC().z * gamma;
}

void TriangleController::fillNormalizedVectorsAndZTable() {
  PixelsTriangle T;
  Triangle t;
  std::vector<Point> inside;
  for (int i = 0; i < pixelsTriangles.size(); i++) {
    T = pixelsTriangles[i];
    t = triangles[i];
    auto pointsT = T.getPoints();

    if (i == 24) {
      int qefwregr = 0;
    }
    normalizedNormalvectors[pointsT[0].y][pointsT[0].x] = t.A.N;
    normalizedNormalvectors[pointsT[1].y][pointsT[1].x] = t.B.N;
    normalizedNormalvectors[pointsT[2].y][pointsT[2].x] = t.C.N;
    Zs[pointsT[0].y][pointsT[0].x] = t.A.z;
    Zs[pointsT[1].y][pointsT[1].x] = t.B.z;
    Zs[pointsT[2].y][pointsT[2].x] = t.C.z;
    alreadySetted[pointsT[0].y][pointsT[0].x] = true;
    alreadySetted[pointsT[1].y][pointsT[1].x] = true;
    alreadySetted[pointsT[2].y][pointsT[2].x] = true;
    int triangleArea = T.getArea();
    inside = getPointsInsideTriangle(pointsT[0], pointsT[1], pointsT[2]);
    for (int k = 0; k < inside.size(); k++) {
      if (k == 8000) {
        int wdefev = 0;
      }
      Point p = inside[k];
      if (!alreadySetted[p.y][p.x]) {
        getNewNUandZpdated(p.x, p.y, pointsT, triangleArea, t);
        alreadySetted[p.y][p.x] = true;
      }
    }
  }
}

TriangleController::TriangleController(int w, int h, const std::vector<Triangle>& tris, int c,
                                       int r)
    : width(w), height(h), triangles(tris) {
  normalizedNormalvectors = new Vector3D*[height+1];
  Zs = new double*[height + 1];
  alreadySetted = new bool*[height + 1];

  for (int i = 0; i < height+1; ++i) {
    normalizedNormalvectors[i] = new Vector3D[width+1];
    Zs[i] = new double[width + 1];
    alreadySetted[i] = new bool[width + 1]{false};
  }
  createPixelsTriangles(c, r);
  fillNormalizedVectorsAndZTable();

}

void TriangleController::setNormalMapImage(QSharedPointer<QImage> nmapIm) {
  this->normalMapImage = nmapIm;
}

//void TriangleController::FillTriangles(QSharedPointer<QImage> image,
//                                       Vector3D lightpos, bool isIm,
//                                       QSharedPointer<QImage> uplIm,
//                                       bool isNMap) {
//  lightPosition = lightpos;
//  Triangle t;
//  ScanLine s(width, height, normalizedNormalvectors,Zs, image, uplIm, isNMap, normalMapImage);
//  for (int i = 0; i < pixelsTriangles.size(); i++) {
//    int tmp = std::min(pixelsTriangles[i].getPoints()[0].y,
//                       pixelsTriangles[i].getPoints()[1].y);
//    int start = std::min(tmp, pixelsTriangles[i].getPoints()[2].y);
//    tmp = std::max(pixelsTriangles[i].getPoints()[0].y,
//                   pixelsTriangles[i].getPoints()[1].y);
//    int end = std::max(tmp, pixelsTriangles[i].getPoints()[2].y);
//    int L = triangles[i].Id / (2 * num_cols);
//    s.configureScanLine(pixelsTriangles[i], start  /*+L*rowStep*/, end /*+ L*rowStep*/, triangles[i]);
//    s.Fill(lightpos, isIm);
//  }
//}


void TriangleController::FillTriangles(QSharedPointer<QImage> image,
                                       Vector3D lightpos, bool isIm,
                                       QSharedPointer<QImage> uplIm,
                                       bool isNMap) {
  lightPosition = lightpos;
  Triangle t;
  ScanLine s(width, height, normalizedNormalvectors, Zs, image, uplIm, isNMap,
             normalMapImage);


   std::vector<std::pair<Triangle, PixelsTriangle>> pairSequence;
  pairSequence.reserve(triangles.size());

  for (size_t i = 0; i < triangles.size(); ++i) {
    pairSequence.emplace_back(triangles[i], pixelsTriangles[i]);
  }
  auto processPair = [&](const std::pair<Triangle, PixelsTriangle>& pair) {
    const PixelsTriangle& pixelTriangle = pair.second;
    const Triangle& triangle = pair.first;

    int tmp = std::min(pixelTriangle.getPoints()[0].y,
                       pixelTriangle.getPoints()[1].y);
    int start = std::min(tmp, pixelTriangle.getPoints()[2].y);
    tmp = std::max(pixelTriangle.getPoints()[0].y,
                   pixelTriangle.getPoints()[1].y);
    int end = std::max(tmp, pixelTriangle.getPoints()[2].y);
    int L = triangle.Id / (2 * num_cols);

    ScanLine s_local(width, height, normalizedNormalvectors, Zs, image, uplIm,
                     isNMap, normalMapImage);
    s_local.configureScanLine(pixelTriangle, start, end, triangle);
    s_local.Fill(lightpos, isIm);
  };
  int testqe = 0;
  QtConcurrent::map(pairSequence, processPair).waitForFinished();
}

PixelsTriangle::PixelsTriangle(Point a, Point b, Point c) {
  points.push_back(a);
  points.push_back(b);
  points.push_back(c);
}

double PixelsTriangle::getArea() {
  double area = 0.5 * std::fabs(points[0].x * (points[1].y - points[2].y) +
                points[1].x * (points[2].y - points[0].y) +
                          points[2].x * (points[0].y - points[1].y));

  return area;
}

std::vector<Point> Polygon::getPoints() const { return points; }

void ScanLine::createEdges() {
  edges = new Edge[points.size()];
  for (int i = 0; i < points.size() - 1; i++) {
    edges[i].start.x = points[i].x;
    edges[i].start.y = points[i].y - start;
    edges[i].end.x = points[i + 1].x;
    edges[i].end.y = points[i + 1].y - start;
  }
  edges[points.size() - 1].start.x = points[points.size() - 1].x;
  edges[points.size() - 1].start.y = points[points.size() - 1].y - start;
  edges[points.size() - 1].end.x = points[0].x;
  edges[points.size() - 1].end.y = points[0].y - start;
}

bool sortByXMin(const ETNode& a, const ETNode& b) { return a.x_min < b.x_min; }

void ScanLine::createET() {
  bucketSortedEdgeTable = new std::vector<ETNode>*[length+1];
  for (int i = 0; i <= length; ++i) {
    bucketSortedEdgeTable[i] = new std::vector<ETNode>();
  }
  Edge edge;
  int /*yMaxScreen, yMinScreen,*/ yMax, yMin, xOfMinY;
  for (int i = 0; i < points.size(); i++) {
    edge = edges[i];
    if (edge.start.y == edge.end.y) continue;
    ETNode* newNode = new ETNode();
    newNode->edge = edge;
    yMax = (edge.start.y > edge.end.y) ? edge.start.y : edge.end.y;
    yMin = (edge.start.y == yMax) ? edge.end.y : edge.start.y;
    //yMax = height - yMinScreen; 
    //yMin = height - yMaxScreen; 
    xOfMinY = (edge.start.y == yMin) ? edge.start.x : edge.end.x;
    newNode->y_max = yMax;
    newNode->x_min = xOfMinY;
    newNode->m = (edge.end.x - edge.start.x) / (edge.end.y - edge.start.y);
    bucketSortedEdgeTable[yMin]->push_back(*newNode);
    ++numOfAddedEdges;
  }
  for (int i = 0; i <= length; ++i) {
    if (!bucketSortedEdgeTable[i]->empty()) {
      std::sort(bucketSortedEdgeTable[i]->begin(),
                bucketSortedEdgeTable[i]->end(), sortByXMin);
    }
  }
}

ScanLine::ScanLine(int width, int height, Vector3D** arr, double** zs,
                   QSharedPointer<QImage> image, QSharedPointer<QImage> uplIm,
                   bool isNmap, QSharedPointer<QImage> nMapImage)
    : normalizedNormalvectors(arr), Zs(zs) {
  this->width = width;
  this->height = height;
  this->image = image;
  this->uploadedImage = uplIm;
  this->normalMapImage = nMapImage;
  this->isNMap = isNmap;
}

void ScanLine::configureScanLine(PixelsTriangle p, int st, int en, Triangle t) {
  pixelsTriangle =
      PixelsTriangle(p.getPoints()[0], p.getPoints()[1], p.getPoints()[2]);
  triangle = t;
  points = p.getPoints();
  triangleArea = p.getArea();
  start = st;
  length = abs(en - st);
  numOfAddedEdges = 0;
  createEdges();
  createET();
}

void ScanLine::FillBetweenXs(int y, int x1, int x2, Vector3D lightPos,
                             bool isIm) {
  int red, green, blue;

  for (int x = x1; x <= x2; x++) {
    Vector3D Nxy = normalizedNormalvectors[height - y][x];
    ///
    if (isNMap) {
      QRgb pixelColor = normalMapImage->pixelColor(x, y).rgb();
      red = qRed(pixelColor);
      green = qGreen(pixelColor);
      blue = qBlue(pixelColor);
      Nxy = Nxy.normalVectorWithNormalMap(
          Nxy,
          Vector3D((red - 128.0) / (double)128.0, (green - 128.0) / (double)128.0,
                   (blue - 128.0) / (double)128.0));
    }
    double z = Zs[height - y][x];
    Vector3D vectorToLight(lightPos.X() - (x / (double)width),
                           lightPos.Y() - (y / (double)height), lightPos.Z() - z);
    RGB imRgb;
    if (isIm) {
      QRgb pixelColor = uploadedImage->pixelColor(x, y).rgb();
      red = qRed(pixelColor);
      green = qGreen(pixelColor);
      blue = qBlue(pixelColor);
      imRgb = RGB(red / (double)255, green / (double)255, blue / (double)255);
    }

    RGB rgb = colorController.getColor(Nxy, vectorToLight, isIm, imRgb);
    red = static_cast<int>(rgb.R() * 255);
    green = static_cast<int>(rgb.G() * 255);
    blue = static_cast<int>(rgb.B() * 255);
    red = qBound(0, red, 255);
    green = qBound(0, green, 255);
    blue = qBound(0, blue, 255);
    QRgb pixelColor = qRgb(red, green, blue);
    image->setPixel(x, y, pixelColor);
  }
}

void ScanLine::Fill(Vector3D lightpos, bool isIm) {
  //int y = start;
  int y = 0;
  int removedET = 0;
  std::vector<ETNode> AET = std::vector<ETNode>();
  AET.clear();
  while (!AET.empty() || removedET != numOfAddedEdges) {
    
    AET.insert(AET.end(), bucketSortedEdgeTable[y]->begin(),
               bucketSortedEdgeTable[y]->end());
    std::sort(AET.begin(), AET.end(), sortByXMin);
    for (int i = 0; i < AET.size() - 1; i++) {
      FillBetweenXs(height - start - y, AET[i].x_min, AET[i + 1].x_min,
                    lightpos, isIm);
      i += 2;
    }
    auto originalSize = AET.size();
    AET.erase(
        std::remove_if(AET.begin(), AET.end(),
                       [y](const ETNode& node) { return isYMax(node, y); }),
        AET.end());

    auto elementsErased = originalSize - AET.size();
    removedET += elementsErased;
    ++y;

    for (auto& node : AET) {
      if (!std::fabs(node.m) <= epsilon) {
        node.x_min += 1 / node.m;
      }
    }
  }
}
