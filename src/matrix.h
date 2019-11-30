/*
*   Definitions for Matrix manipulation routines
*   Author: Ioannis Karamouzas, 10/15/18
*   The code is based on previous code from D. House
*/

#include <cstdio>
#include <cmath>

#ifndef PI
#define PI		3.1415926536
#endif

struct Vector3D{
  double x, y, z;
  Vector3D(): x(0),y(0),z(1) {}
  Vector3D(double x_, double y_, double z_): x(x_), y(y_), z(z_) {}
};

struct Vector2D{
  double x, y;
  Vector2D(): x(0),y(0) {}
  Vector2D(double x_, double y_): x(x_), y(y_) {}
};

class Matrix3D{
private:
  double M[3][3];
  
public:
  Matrix3D();
  Matrix3D(const double coefs[3][3]);
  Matrix3D(const Matrix3D &mat);

  void print() const;

  void setidentity();
  void set(const double coefs[3][3]);
  
  double determinant() const;
  Matrix3D adjoint() const;
  Matrix3D inverse() const;
  
  Vector3D operator*(const Vector2D &v) const;
  Vector3D operator*(const Vector3D &v) const;
  Matrix3D operator*(const Matrix3D &m2) const;
  double *operator[](int i);
};

struct BilinearCoeffs{
  double width, height;
  double a0, a1, a2, a3;
  double b0, b1, b2, b3;
  double c2;
};

void setbilinear(double width, double height,
		 Vector2D xycorners[4], BilinearCoeffs &coeff);
void invbilinear(const BilinearCoeffs &c, Vector2D xy, Vector2D &uv);
