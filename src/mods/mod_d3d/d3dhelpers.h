
#ifndef D3D_HELPERS_H
#define D3D_HELPERS_H

// Acquires an additional reference, if non-nullptr.
template <typename InterfaceType>
inline InterfaceType* SafeAcquire(InterfaceType* newObject)
{
    if (newObject != nullptr)
        newObject->AddRef();

    return newObject;
}

// Releases a reference, if non-nullptr
template <class T> inline void SafeRelease(T **ppT)
{
   if ( *ppT )
   {
      (*ppT)->Release();
      *ppT = nullptr;
   }
}

#endif // D3D_HELPERS_H
