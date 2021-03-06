
#ifndef MATRIX_4X4_H
#define MATRIX_4X4_H

#include "core/core_base.h"

class Vector;

#ifdef WIN32
__declspec(align(16))
#endif

class CORE_API Matrix4
{
public:
   Matrix4();
   Matrix4(const float* pmatrix);
   Matrix4(float xx, float xy, float xz, float xw,
           float yx, float yy, float yz, float yw,
           float zx, float zy, float zz, float zw,
           float wx, float wy, float wz, float ww);

   const Matrix4& operator=(const Matrix4& that);
   bool operator!=(const Matrix4& that) const;

   void asOpenGL(float mat[16]) const;

   void setOrtho(float width, float height, float znear, float zfar);

   void setIdentity();
   void setArray(const float* pmatrix);
   void setTranslation(float x, float y, float z);

   //Matrix4 transpose();
      
   Matrix4& translate(float x, float y, float z);
   Matrix4& translate(const Vector& position);

   Matrix4& rotateZ(float angle);

private:
   // row major order
   float mMatrix[16];
};

#endif // MATRIX_4X4_H
