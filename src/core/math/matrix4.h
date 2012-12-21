
#ifndef MATRIX_4X4_H
#define MATRIX_4X4_H

#include "core/core_base.h"

class CORE_API Matrix4
{
public:
   Matrix4();
   Matrix4(const float* pmatrix);

   const Matrix4& operator=(const Matrix4& that);
   bool operator!=(const Matrix4& that) const;

   const float* asArray() const;

   void setIdentity();
   void setArray(const float* pmatrix);

private:
   float mMatrix[16];
};

#endif // MATRIX_4X4_H
