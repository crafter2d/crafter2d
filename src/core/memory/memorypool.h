
#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_

#include <queue>

template<class T>
class MemoryPool
{
   typedef std::queue<T*> Pool;

public:
   MemoryPool(): mPool() {}

   T* alloc() {
      if ( mPool.empty() )
      {
         return new T();
      }
      else
      {
         T* pitem = mPool.front();
         mPool.pop();
         return pitem;
      }
   }

   void free(T* pitem) {
      mPool.push(pitem);
   }

private:
   Pool mPool;
};

#endif // MEMORY_POOL_H_
