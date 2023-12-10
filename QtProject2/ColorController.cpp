#include "ColorController.h"

RGB::RGB(double _r, double _g, double _b) {
  r = _r;
  g = _g;
  b = _b;
}

double RGB::R() { return r; }
double RGB::G() { return g; }
double RGB::B() { return b; }

double ColorController::kd = 0.3;
double ColorController::ks = 0.6;
int ColorController::m = 50;
RGB ColorController::objectColor = RGB(0.4, 0.8, 1);
RGB ColorController::lightColor = RGB(1.0, 1.0, 1.0);
bool ColorController::isImage = false;

void ColorController::setKd(double a) { kd = a; }

void ColorController::setKs(double a) { ks = a; }

void ColorController::setM(int _m) { m = _m; }

void ColorController::setLightColor(RGB newColor) {
  lightColor = RGB(newColor.R(), newColor.G(), newColor.B());
}

void ColorController::setObjectColor(RGB newColor) {
  objectColor = RGB(newColor.R(), newColor.G(), newColor.B());
}

RGB ColorController::getColor(Vector3D N, Vector3D L, bool isIm, RGB imRgb) { 
    isImage = isIm;
	Vector3D normalizedN = N.getNormalized();
	Vector3D normalizedL = L.getNormalized();   

    Vector3D R_first = normalizedN.scalarMultiply(2 * normalizedN.dotProduct(normalizedL));
    Vector3D R = R_first.subtract(normalizedL);
    Vector3D normalizedR = R.getNormalized();

    double cos_v = normalizedN.dotProduct(normalizedL);
    double cos_val1 = (cos_v > 0) ? cos_v : 0;
    cos_v = V.dotProduct(normalizedR);
    double cos_val2 = (cos_v > 0) ? cos_v : 0;

    double I_r, I_g, I_b;
    if (isImage) {
     I_r = kd * lightColor.R() * imRgb.R() * cos_val1 +
           ks * lightColor.R() * imRgb.R() * pow(cos_val2, m);
     I_g = kd * lightColor.G() * imRgb.G() * cos_val1 +
           ks * lightColor.G() * imRgb.G() * pow(cos_val2, m);
     I_b = kd * lightColor.B() * imRgb.B() * cos_val1 +
                   ks * lightColor.B() * imRgb.B() * pow(cos_val2, m);
    } else {
      I_r = kd * lightColor.R() * objectColor.R() * cos_val1 +
            ks * lightColor.R() * objectColor.R() * pow(cos_val2, m);
      I_g = kd * lightColor.G() * objectColor.G() * cos_val1 +
            ks * lightColor.G() * objectColor.G() * pow(cos_val2, m);
      I_b = kd * lightColor.B() * objectColor.B() * cos_val1 +
                   ks * lightColor.B() * objectColor.B() * pow(cos_val2, m);
    }
	return RGB(I_r, I_g, I_b); 
}
