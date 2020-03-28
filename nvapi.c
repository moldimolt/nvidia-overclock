/*
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004
 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include "nvapi.h"

NvAPI_QueryInterface_t NvQueryInterface = NvQueryInterface_trampoline;
NvAPI_Initialize_t NvInit = NvInit_trampoline;
NvAPI_Unload_t NvUnload = NvUnload_trampoline;
NvAPI_EnumPhysicalGPUs_t NvEnumGPUs = NvEnumGPUs_trampoline;
NvAPI_GPU_GetSystemType_t NvGetSysType = NvGetSysType_trampoline;
NvAPI_GPU_GetFullName_t NvGetName = NvGetName_trampoline;
NvAPI_GPU_GetPhysicalFrameBufferSize_t NvGetMemSize = NvGetMemSize_trampoline;
NvAPI_GPU_GetRamType_t NvGetMemType = NvGetMemType_trampoline;
NvAPI_GPU_GetVbiosVersionString_t NvGetBiosName = NvGetBiosName_trampoline;
NvAPI_GPU_GetAllClockFrequencies_t NvGetFreq = NvGetFreq_trampoline;
NvAPI_GPU_GetPstates20_t NvGetPstates = NvGetPstates_trampoline;
NvAPI_GPU_SetPstates20_t NvSetPstates = NvSetPstates_trampoline;

void *NvQueryInterface_trampoline(unsigned int offset)
{
    NvQueryInterface = (void*)GetProcAddress(LoadLibrary("nvapi.dll"), "nvapi_QueryInterface");
    return NvQueryInterface(offset);
}

int NvInit_trampoline(void)
{
    NvInit = NvQueryInterface(0x0150E828);
    return NvInit();
}

int NvUnload_trampoline(void)
{
    NvUnload = NvQueryInterface(0xD22BDD7E);
    return NvUnload();
}

int NvEnumGPUs_trampoline(int **handles, int *count)
{
    NvEnumGPUs = NvQueryInterface(0xE5AC921F);
    return NvEnumGPUs(handles, count);
}

int NvGetSysType_trampoline(int *handle, int *systype)
{
    NvGetSysType = NvQueryInterface(0xBAAABFCC);
    return NvGetSysType(handle, systype);
}

int NvGetName_trampoline(int *handle, char *sysname)
{
    NvGetName = NvQueryInterface(0xCEEE8E9F);
    return NvGetName(handle, sysname);
}

int NvGetMemSize_trampoline(int *handle, int *memsize)
{
    NvGetMemSize = NvQueryInterface(0x46FBEB03);
    return NvGetMemSize(handle, memsize);
}

int NvGetMemType_trampoline(int *handle, int *memtype)
{
    NvGetMemType = NvQueryInterface(0x57F7CAAC);
    return NvGetMemType(handle, memtype);
}

int NvGetBiosName_trampoline(int *handle, char *biosname)
{
    NvGetBiosName = NvQueryInterface(0xA561FD7D);
    return NvGetBiosName(handle, biosname);
}

int NvGetFreq_trampoline(int *handle, NV_GPU_CLOCK_FREQUENCIES_V2 *clock_info)
{
    NvGetFreq = NvQueryInterface(0x0DCB616C3);
    return NvGetFreq(handle, clock_info);
}

int NvGetPstates_trampoline(int *handle, NV_GPU_PERF_PSTATES20_INFO_V1 *pstates_info)
{
    NvGetPstates = NvQueryInterface(0x6FF81213);
    return NvGetPstates(handle, pstates_info);
}

int NvSetPstates_trampoline(int *handle, int *pstates_info)
{
    NvSetPstates = NvQueryInterface(0x0F4DAE6B);
    return NvSetPstates(handle, pstates_info);
}