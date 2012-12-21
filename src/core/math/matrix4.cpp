
#include "matrix4.h"

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

const float* Matrix4::asArray() const
{
   return mMatrix;
}

void Matrix4::setIdentity()
{
	mMatrix[0] = 1.0f;
	mMatrix[1] = 0.0f;
	mMatrix[2] = 0.0f;
	mMatrix[3] = 0.0f;

	mMatrix[4] = 0.0f;
	mMatrix[5] = 1.0f;
	mMatrix[6] = 0.0f;
	mMatrix[7] = 0.0f;

	mMatrix[8] = 0.0f;
	mMatrix[9] = 0.0f;
	mMatrix[10] = 1.0f;
	mMatrix[11] = 0.0f;

	mMatrix[12] = 0.0f;
	mMatrix[13] = 0.0f;
	mMatrix[14] = 0.0f;
	mMatrix[15] = 1.0f;
}

void Matrix4::setArray(const float* pmatrix)
{
   for ( int index = 0; index < 16; ++index )
	{
		mMatrix[index] = pmatrix[index];
	}
}
