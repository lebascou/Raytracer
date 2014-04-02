#pragma once

#include "SSE.h"

struct WSSEALIGN SSEVector4
{
    __m128 x, y, z;
} GSSEALIGN;

class WSSEALIGN SSEObject
{
public:
    inline void *operator new[](size_t sz) { return _mm_malloc(sz, 16); }
    inline void *operator new(size_t sz) { return _mm_malloc(sz, 16); }
    inline void operator delete[](void *p) { if (p) _mm_free(p); }
    inline void operator delete(void *p) { if (p) _mm_free(p); }
} GSSEALIGN;
