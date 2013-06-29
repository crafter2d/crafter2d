
#include "vertexinputlayout.h"

#include "core/log/log.h"
#include "core/defines.h"

#include "vertexinputelement.h"

namespace Graphics
{

   VertexInputLayout::VertexInputLayout(int fvf):
      mpElements(NULL),
      mCount(0),
      mStride(0)
   {
      createElements(fvf);
   }

   VertexInputLayout::~VertexInputLayout()
   {
      delete[] mpElements;
      mpElements = NULL;
   }
   
   // - Statics

   /*!
    \fn VertexInputLayout::calculateFieldCount(int fvf)
	 \brief Determines the number of vertex attributes in the attribute parameter
	 \param[in] fvf the vertex attribute flag which was used to create the vertex buffer
	 \return The number of attributes included in the flag
 */
   int VertexInputLayout::calculateFieldCount(int fvf)
   {
	   int count = 0;
	   for (int i = 1; i < fvf; i <<= 1) {
		   if (fvf & i)
			   count++;
	   }
	   return count;
   }

   /*!
    \fn VertexInputLayout::calculateStride(int fvf)
	 \brief Calculates the complete size in bytes of all the vertex attribute in the flag
	 \param[in] fvf he vertex attribute flag which was used to create the vertex buffer
	 \return The total size in bytes of all vertex attributes
   */
   int VertexInputLayout::calculateStride(int fvf)
   {
	   int size = 0;
	   for ( int i = 1; i < fvf; i<<=1 )
      {
	      switch (fvf & i) {
	      case INPUT_XYZW:
	      case INPUT_Diffuse:
	      case INPUT_Specular:
		      size += sizeof(float) * 4;
		      break;
	      case INPUT_XYZ:
	      case INPUT_Normal:
	      case INPUT_Tangent:
	      case INPUT_Binormal:
		      size += sizeof(float) * 3;
		      break;
	      case INPUT_XY:
	      case INPUT_Tex0:
	      case INPUT_Tex1:
	      case INPUT_Tex2:
	      case INPUT_Tex3:
	      case INPUT_Tex4:
		      size += sizeof(float) * 2;
		      break;
	      default:
		      break;
	      }
      }
      return size;
   }

   const VertexInputElement& VertexInputLayout::operator[](int index) const
   {
      ASSERT(index < mCount);
      return mpElements[index];
   }

   // - Get/set

   int VertexInputLayout::getSize() const
   {
      return mCount;
   }

   int VertexInputLayout::getStride() const
   {
      return mStride;
   }

   // - Operations

   /*!
    \fn VertexInputLayout::createElements(int fvf)
	 \brief Fills in the buffer descriptor with the required information.
	 \param[in] fvf the vertex attributes that where used to create the vertex buffer
	 \param[in] descs array of VertexBufferDesc structures
	 \return Nothing
 */
   void VertexInputLayout::createElements(int fvf)
   {
      mCount = calculateFieldCount(fvf);
      mStride = calculateStride(fvf);

      mpElements = new VertexInputElement[mCount];
      if ( mpElements == NULL )
	   {
         Log::getInstance().error("Input Layout - Critical Error - out of memory.\n");
         return;
	   }

      int pos = 0;
	   int cnt = 0, i = 1;

	   while (i <= fvf)
	   {
		   if (!(i & fvf))
		   {
			   i <<= 1;
			   continue;
		   }

		   VertexInputElement& field = mpElements[cnt];

		   field.pos = pos;
		   field.flags = i;
		   switch ( fvf & i )
         {
		   case INPUT_XY:
            field.semantic = "POSITION";
			   field.index = 0;
			   field.size = 2;
			   break;
		   case INPUT_XYZ:
            field.semantic = "POSITION";
			   field.index = 0;
			   field.size = 3;
			   break;
		   case INPUT_XYZW:
            field.semantic = "POSITION";
			   field.index = 0;
			   field.size = 4;
			   break;
		   case INPUT_Normal:
            field.semantic = "NORMAL";
			   field.index = 2;
			   field.size = 3;
			   break;
		   case INPUT_Diffuse:
			   field.index = 3;
			   field.size = 4;
			   break;
		   case INPUT_Specular:
			   field.index = 4;
			   field.size = 4;
			   break;
		   case INPUT_Tex0:
            field.semantic = "TEXCOORD0";
			   field.index = 8;
			   field.size = 2;
			   break;
		   case INPUT_Tex1:
			   field.index = 9;
			   field.size = 2;
			   break;
		   case INPUT_Tex2:
			   field.index = 10;
			   field.size = 2;
			   break;
		   case INPUT_Tex3:
			   field.index = 11;
			   field.size = 2;
			   break;
		   case INPUT_Tex4:
			   field.index = 12;
			   field.size = 2;
			   break;
		   case INPUT_Tangent:
			   field.index = 14;
			   field.size = 3;
			   break;
		   case INPUT_Binormal:
			   field.index = 15;
			   field.size = 3;
			   break;
		   default:
			   break;
		   }

		   pos += field.size * sizeof(float);
		   i <<= 1;
		   cnt++;
	   }
   }

} // namespace Graphics

