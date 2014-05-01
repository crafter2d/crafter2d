
#ifndef D3D_HELPERS_H
#define D3D_HELPERS_H

// Acquires an additional reference, if non-null.
template <typename InterfaceType>
inline InterfaceType* SafeAcquire(InterfaceType* newObject)
{
    if (newObject != NULL)
        newObject->AddRef();

    return newObject;
}

// Releases a reference, if non-null
template <class T> inline void SafeRelease(T **ppT)
{
   if ( *ppT )
   {
      (*ppT)->Release();
      *ppT = NULL;
   }
}

#endif // D3D_HELPERS_H
