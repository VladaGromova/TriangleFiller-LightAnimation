#include "WindowController.h"

void WindowController::drawGrid() {
  QPainter painter(image.data());
  painter.setPen(Qt::black);
  std::vector<PixelsTriangle> pixTr = triangleController.getPixelsTriangles();
  for (const auto& triangle : pixTr) {
    const std::vector<Point>& points = triangle.getPoints();
    if (points.size() != 3) {
      continue;
    }
    std::array<QPoint, 3> qPoints;
    for (size_t ind = 0; ind < 3; ++ind) {
      qPoints[ind] = QPoint(points[ind].x, points[ind].y);
    }
    painter.drawPolygon(qPoints.data(), 3);
    QPoint topLeft = qPoints[0];
    QPoint topRight = qPoints[1];
    QPoint bottomLeft = qPoints[2];
    painter.drawLine(topRight, bottomLeft);
  }
  painter.end();
}

void WindowController::drawControlPoints() {
  QPainter painter(image.data());
  QPen redPen(Qt::red);
  redPen.setWidth(1);
  painter.setPen(redPen);
  Vector3D** vectors = controlPointsController->getMainVectors();
  double centerX, centerY;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      centerX = vectors[i][j].X() * width;
      centerY = vectors[i][j].Y() * height;
      painter.drawEllipse(QPointF(centerX, centerY), radius, radius);
    }
  }
  if (z_i != -1 && z_j != -1) {
    QPen yellowPen(Qt::yellow);
    yellowPen.setWidth(2);
    painter.setPen(yellowPen);
    centerX = vectors[z_i][z_j].X() * width;
    centerY = vectors[z_i][z_j].Y() * height;
    painter.drawEllipse(QPointF(centerX, centerY), radius, radius);
  }
  painter.end();
}

void WindowController::showControlPoints() { 
  qDebug() << "[I'M HERE] WindowController showControlPoints called \n";
  isControlPoints = true;
  drawScene();
  emit controlPointsShowed();
}

void WindowController::hideControlPoints() {
  qDebug() << "[I'M HERE] WindowController hideControlPoints called \n";
  isControlPoints = false;
  drawScene();
  emit controlPointsHiden();
}

void WindowController::hideGrid() {
  qDebug() << "[I'M HERE] WindowController hideGrid called \n";
  isGrid = false;
  drawScene();
  emit gridHiden();
}

void WindowController::showGrid() {
  qDebug() << "[I'M HERE] WindowController showGrid called \n";
  isGrid = true;
  drawScene();
  emit gridShowed();
}

void WindowController::drawFilledTriangles() {
  triangleController.FillTriangles(image, lightPosition, isImage, uploadedImage, isNormalMap);
}

void WindowController::drawScene() { 
    drawFilledTriangles(); 
    if (isGrid) {
      drawGrid();
    }
    if (isControlPoints) {
      drawControlPoints();
    }
}

WindowController::WindowController(QObject* parent) : QObject(parent) {
  image = QSharedPointer<QImage>::create(width, height, QImage::Format_ARGB32);
  uploadedImage =
      QSharedPointer<QImage>::create(width, height, QImage::Format_ARGB32);
  normalMapImage =
      QSharedPointer<QImage>::create(width, height, QImage::Format_ARGB32);

  uploadedImage->load(FILEPATH);
  *uploadedImage = uploadedImage->scaled(uploadedImage->size().boundedTo(QSize(width, height)));

  normalMapImage->load(NORMAL_MAP_PATH);
  *normalMapImage = normalMapImage->scaled(normalMapImage->size().boundedTo(QSize(width, height)));

  controlPointsController = new BezierController();
  grid = TriangleGrid(*controlPointsController, accuracy, accuracy);
  std::vector<Triangle> triangles = grid.getTriangles();
  triangleController = TriangleController(width, height, triangles, accuracy, accuracy);
  triangleController.setNormalMapImage(normalMapImage);
  drawScene();
}

void WindowController::setGridAccuracy(int value) {
  accuracy = pow(2,value);
  grid = TriangleGrid(*controlPointsController, accuracy, accuracy);
  std::vector<Triangle> triangles = grid.getTriangles();
  triangleController =
      TriangleController(width, height, triangles, accuracy, accuracy);
  triangleController.setNormalMapImage(normalMapImage);
  drawScene();
  emit gridAccuracySetted();
}

void WindowController::setKd(double value) { 
    ColorController::setKd(value); 
    drawScene();
    emit kdSetted();
}

void WindowController::setKs(double value) {
  ColorController::setKs(value);
  drawScene();
  emit ksSetted();
}

void WindowController::setM(int value) {
  ColorController::setM(value);
  drawScene();
  emit mSetted();
}

void WindowController::changeBackgroundColor(const QColor& color) {
  RGB newColor(color.red() / (double)255, color.green() / (double)255, color.blue() / (double)255);
  isImage = false;
  ColorController::setObjectColor(newColor);
  drawScene();
  emit backgroundColorChanged();
}

void WindowController::changeLightColor(const QColor& color) {
  RGB newColor(color.red() / (double)255, color.green() / (double)255,
               color.blue() / (double)255);
  ColorController::setLightColor(newColor);
  drawScene();
  emit lightColorChanged();
}

void WindowController::setLightHeight(double value) {
  lightPosition.setZ(value);
  drawScene();
  emit lightHeightSetted();
}

void WindowController::chooseZ(double _x, double _y) {
  Vector3D** vectors = controlPointsController->getMainVectors();
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      double centerX = vectors[i][j].X();
      double centerY = vectors[i][j].Y();
      if (std::fabs(centerX - _x) < radius / (double)width &&
          std::fabs(centerY - _y) < radius
        / (double) height) {
        z_i = i;
        z_j = j;
      }
    }
  }

  drawScene();
  emit zChoosed();
}

void WindowController::setBackgroundImage(const QString& filePath) {
  QUrl url(filePath);
  QString localPath = url.toLocalFile();
  localPath.replace("/", "\\");

  isImage = true;
  if (uploadedImage->load(localPath)) {
    *uploadedImage = uploadedImage->scaled(
        uploadedImage->size().boundedTo(QSize(width, height)));
  } else {
    qDebug() << "Failed to load the image from" << localPath;
  }
  drawScene();
  emit backgroundImageSetted();
}

void WindowController::modifyNormalMap() { 
    isNormalMap = true;
    drawScene();
    emit normalMapModified();
}

void WindowController::removeNormalMap() {
  isNormalMap = false;
  drawScene();
  emit normalMapRemoved();
}

void WindowController::setNormalMap(const QString& filePath) {
  QUrl url(filePath);
  QString localPath = url.toLocalFile();
  localPath.replace("/", "\\");

  normalMapImage->load(localPath);

  QSize originalSize = normalMapImage->size();

  if (originalSize.width() < width || originalSize.height() < height) {
    *normalMapImage =
        normalMapImage->scaled(width, height, Qt::IgnoreAspectRatio);
  } else {
    *normalMapImage = normalMapImage->scaled(
        normalMapImage->size().boundedTo(QSize(width, height)));
  }

  triangleController.setNormalMapImage(normalMapImage);
  drawScene();
  emit normalMapSetted();
}

void WindowController::setDefaultNormalMap() {
  normalMapImage->load(NORMAL_MAP_PATH);
  *normalMapImage = normalMapImage->scaled(
      normalMapImage->size().boundedTo(QSize(width, height)));

  triangleController.setNormalMapImage(normalMapImage);
  drawScene();
  emit normalMapSetted();
}

void WindowController::setZValue(double new_value) {
  if (z_i != -1 && z_j != -1) {
    Vector3D** vectors = controlPointsController->getMainVectors();
    vectors[z_i][z_j].setZ(new_value);

    grid = TriangleGrid(*controlPointsController, accuracy, accuracy);
    std::vector<Triangle> triangles = grid.getTriangles();
    triangleController =
        TriangleController(width, height, triangles, accuracy, accuracy);
    triangleController.setNormalMapImage(normalMapImage);
    
    drawScene();
    emit zValueSetted();
  }
}

void WindowController::animate() {
  animation = true;
  connect(this, &WindowController::animationStopped,
          [&]() { animation = false; });
  QFuture<void> future = QtConcurrent::run([=]() {
    while (true) {
      if (!animation) {
        break;
      }
      QCoreApplication::processEvents();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  });
}

void WindowController::stopAnimation() {
  animation = false;
  emit animationStopped();
}

void WindowController::moveLight() {
  // if (animation) {
  double x = 0.3 + 0.5 * std::cos(angle);
  double y = 0.3 + 0.5 * std::sin(angle);
  lightPosition.setX(x);
  lightPosition.setY(y);
  angle += 0.2;
  //}
  drawScene();
  emit lightMoved();
}