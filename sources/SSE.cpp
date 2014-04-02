#include "SSE.h"

#ifdef _WIN32
#include <intrin.h>
#define cpuid __cpuid
#else
void cpuid(int CPUInfo[4],int InfoType){
    __asm__ __volatile__ (
        "cpuid":
        "=a" (CPUInfo[0]),
        "=b" (CPUInfo[1]),
        "=c" (CPUInfo[2]),
        "=d" (CPUInfo[3]) :
        "a" (InfoType)
    );
}
#endif

SSE sse_info;

SSE::SSE() : m_hasSSE(false)
{
    int info[4];
    cpuid(info, 0);
    int nIds = info[0];
    if (nIds >= 1)
    {
        cpuid(info, 1);
        // This tests for SSE 4.1. We will probably only use SSE2, if so, change that later.
        m_hasSSE = (info[2] & ((int)1 << 19)) != 0;
    }
}
