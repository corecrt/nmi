#pragma once

#include <stdint.h>

typedef struct _KPRCB {
    uint64_t PrcbPad141[512];
} KPRCB;

typedef struct _KPRCB* PKPRCB;

// functions imported by the target (not useful, just for stack logging)
PKPRCB __fastcall KeGetCurrentPrcb(void);
void __fastcall KiFreezeTargetExecution(void);
void __fastcall KiResumeForReboot(void);
void __fastcall VslKernelShadowStackAssist(uint32_t, uint64_t, uint32_t, uint32_t, int64_t, uint32_t);
void __fastcall KiWaitForReboot(void);
uint64_t __fastcall __readmsr(uint32_t);
void __fastcall _mm_lfence(void);
uint64_t __fastcall __readcr3(void);
void* __fastcall ExAllocatePoolWithTag(int, size_t, uint32_t);
void __fastcall ZwProtectVirtualMemory(void*, void**, size_t, uint32_t, uint32_t*);
void* __fastcall KeGetCurrentPrcb(void);
void* __fastcall KxReleaseSpinLock(void*);
int __fastcall KxTryToAcquireSpinLock(void*);
void* __fastcall KiCheckForFreezeExecution(uint64_t, uint64_t);
