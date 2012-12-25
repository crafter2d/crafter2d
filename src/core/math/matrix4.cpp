
#include "matrix4.h"

#include "vector.h"

Matrix4::Matrix4():
	mMatrix()
{
	setIdentity();
}

Matrix4::Matrix4(const float* pmatrix):
	mMatrix()
{
	setArray(pmatrix);
}

Matrix4::Matrix4(float xx, float xy, float xz, float xw,
           float yx, float yy, float yz, float yw,
           float zx, float zy, float zz, float zw,
           float wx, float wy, float wz, float ww):
   mMatrix()
{
   mMatrix[ 0] = xx; mMatrix[ 1] = xy; mMatrix[ 2] = xz; mMatrix[ 3] = xw;
   mMatrix[ 4] = yx; mMatrix[ 5] = yy; mMatrix[ 6] = yz; mMatrix[ 7] = yw;
   mMatrix[ 8] = zx; mMatrix[ 9] = zy; mMatrix[10] = zz; mMatrix[11] = zw;
   mMatrix[12] = wx; mMatrix[13] = wy; mMatrix[14] = wz; mMatrix[15] = ww;
}

const Matrix4& Matrix4::operator=(const Matrix4& that)
{
   setArray(that.mMatrix);
   return *this;
}

bool Matrix4::operator!=(const Matrix4& that) const
{
   for ( int index = 0; index < 16; ++index )
   {
      if ( mMatrix[index] != that.mMatrix[index] )
      {
         return true;
      }
   }
   return false;
}

void Matrix4::asOpenGL(float mat[16]) const
{
   // transpose matrix in there
   mat[ 0] = mMatrix[0]; mat[ 1] = mMatrix[4]; mat[ 2] = mMatrix[ 8]; mat[ 3] = mMatrix[12];
   mat[ 4] = mMatrix[1]; mat[ 5] = mMatrix[5]; mat[ 6] = mMatrix[ 9]; mat[ 7] = mMatrix[13];
   mat[ 8] = mMatrix[2]; mat[ 9] = mMatrix[6]; mat[10] = mMatrix[10]; mat[11] = mMatrix[14];
   mat[12] = mMatrix[3]; mat[13] = mMatrix[7]; mat[14] = mMatrix[11]; mat[15] = mMatrix[15];
}

void Matrix4::setIdentity()
{
   // column-major
	mMatrix[ 0] = 1.0f; mMatrix[ 1] = 0.0f; mMatrix[ 2] = 0.0f; mMatrix[ 3] = 0.0f;
	mMatrix[ 4] = 0.0f; mMatrix[ 5] = 1.0f; mMatrix[ 6] = 0.0f; mMatrix[ 7] = 0.0f;
	mMatrix[ 8] = 0.0f; mMatrix[ 9] = 0.0f; mMatrix[10] = 1.0f; mMatrix[11] = 0.0f;
	mMatrix[12] = 0.0f; mMatrix[13] = 0.0f; mMatrix[14] = 0.0f; mMatrix[15] = 1.0f;
}

void Matrix4::setArray(const float* pmatrix)
{
   mMatrix[ 0] = pmatrix[ 0]; mMatrix[ 1] = pmatrix[ 1]; mMatrix[ 2] = pmatrix[ 2]; mMatrix[ 3] = pmatrix[ 3];
   mMatrix[ 4] = pmatrix[ 4]; mMatrix[ 5] = pmatrix[ 5]; mMatrix[ 6] = pmatrix[ 6]; mMatrix[ 7] = pmatrix[ 7];
   mMatrix[ 9] = pmatrix[ 9]; mMatrix[ 9] = pmatrix[ 9]; mMatrix[10] = pmatrix[10]; mMatrix[11] = pmatrix[11];
   mMatrix[12] = pmatrix[13]; mMatrix[13] = pmatrix[13]; mMatrix[14] = pmatrix[14]; mMatrix[15] = pmatrix[15];
}

void Matrix4::setPosition(const Vector& position)
{
   mMatrix[12] = position.x;
   mMatrix[13] = position.y;
}
