#pragma once
#include <QImage>
#include <QColor>
#include <QObject>
#include <QSharedPointer>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QImageReader>
#include <QString>
#include <QFuture>
class TriangleController;
#include "TriangleController.h"
#define FILEPATH "backroundImage.jpg"
#define NORMAL_MAP_PATH "normalMap.jpg"

class WindowController : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool anim MEMBER animation) //ADDED
 private:
  BezierController* controlPointsController;
  TriangleGrid grid;
  TriangleController triangleController;
  int width = 512;
  int height = 512;
  int accuracy = 4;
  int z_i = -1;
  int z_j = -1;
  double radius = 15;
  double angle = 0.0;
  Vector3D lightPosition = Vector3D(0.5, 0.5, 2.0);
  bool isGrid = false;
  bool isControlPoints = false;
  bool isImage = true;
  bool isNormalMap = false;
  bool isDefaultNormalMap = true;
  void drawGrid();
  void drawControlPoints();
  void drawFilledTriangles();

  std::atomic<bool> animation{false};

 public:
  Q_INVOKABLE void drawScene();
  QSharedPointer<QImage> image;
  QSharedPointer<QImage> uploadedImage;
  QSharedPointer<QImage> normalMapImage;
  QColor color = QColor(Qt::darkBlue);
  explicit WindowController(QObject *parent = nullptr);

  Q_INVOKABLE void animate();

 signals:
  void gridShowed();
  void gridHiden();
  void controlPointsShowed();
  void controlPointsHiden();
  void gridAccuracySetted();
  void kdSetted();
  void ksSetted();
  void mSetted();
  void backgroundColorChanged();
  void lightColorChanged();
  void lightHeightSetted();
  void zChoosed();
  void backgroundImageSetted();
  void normalMapModified();
  void normalMapRemoved();
  void normalMapSetted();
  void zValueSetted();

  void animationStopped(); 
  void lightMoved();

  public slots:
	void showGrid();
    void hideGrid();
    void showControlPoints();
    void hideControlPoints();
    void setGridAccuracy(int value);
    void setKd(double value);
    void setKs(double value);
    void setM(int value);
    void changeBackgroundColor(const QColor& color);
    void changeLightColor(const QColor& color);
    void setLightHeight(double value);
    void chooseZ(double _x, double _y);
    void setBackgroundImage(const QString& filePath);
    void modifyNormalMap();
    void removeNormalMap();
    void setNormalMap(const QString& filePath);
    void setDefaultNormalMap();
    void setZValue(double new_val);

    void stopAnimation();
    void moveLight();
};
