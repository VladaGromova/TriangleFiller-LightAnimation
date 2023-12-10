#pragma once
class BezierController;
#include "BezierController.h"
class RGB
{
private:
  double r, g, b;
 public:
  RGB() = default;
  RGB(double _r, double _g, double _b);
  double R();
  double G();
  double B();
};

class ColorController {
 private:
  static RGB objectColor;
  static RGB lightColor;
  static double kd;
  static double ks;
  static int m;
  Vector3D V = Vector3D(0.0, 0.0, 1.0);
  static bool isImage;
  //QSharedPointer<QImage> image;
 public:
  static void setKd(double a);
  static void setKs(double a);
  static void setM(int _m);
  static void setObjectColor(RGB newColor);
  static void setLightColor(RGB newColor);
  RGB getColor(Vector3D N, Vector3D L, bool isIm, RGB imRgb);
};
