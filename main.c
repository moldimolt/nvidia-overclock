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

#include <stdio.h>
#include <stdlib.h>
#include "nvapi.h"

char *get_memtype_string(int memtype)
{
    switch (memtype)
    {
        case 0:
            return "\0";
            break;
        case 1:
            return "SDR";
            break;
        case 2:
            return "DDR";
            break;
        case 3:
        case 9:
            return "DDR2";
            break;
        case 4:
            return "GDDR2";
            break;
        case 5:
            return "GDDR3";
            break;
        case 6:
            return "GDDR4";
            break;
        case 7:
            return "DDR3";
            break;
        case 8:
            return"GDDR5";
            break;
        case 10:
            return "GDDR5X";
            break;
        case 11:
        case 13:
        default:
            return "Unknown memory type";
            break;
        case 12:
            return "HBM2";
            break;
        case 14:
            return "GDDR6";
            break;
    }
}

int get_memfreq_multiplier(int memtype)
{
    switch(memtype)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 12:
        default:
            return 1;
        case 8:
            return 2;
        case 14:
            return 4;
    }
}

int main(int argc, char **argv)
{
    int nGPU=0;
    int systype;
    int memsize[64], memtype[64];
    int *hdlGPU[64], *buf=0;
    char sysname[64];
    char *biosname[64];
    NV_GPU_PERF_PSTATES20_INFO_V1 pstates_info[64], pstate_overclock;
    NV_GPU_CLOCK_FREQUENCIES_V2 clock_freq[64];

    memset(clock_freq, 0, sizeof(NV_GPU_CLOCK_FREQUENCIES_V2) * 64);

    NvInit();
    NvEnumGPUs(hdlGPU, &nGPU);
    NvGetName(hdlGPU[0], sysname);
    NvGetSysType(hdlGPU[0], &systype);

    for(int i = 0; i < nGPU; i++)
    {
        pstates_info[i].version = 0x11c94;
        biosname[i] = malloc(64);
        NvGetMemSize(hdlGPU[i], &memsize[i]);
        NvGetMemType(hdlGPU[i], &memtype[i]);
        NvGetBiosName(hdlGPU[i], biosname[i]);
        clock_freq[i].version = 0x20108; // set struct version
        clock_freq[i].ClockType = 1; // request base clock
        NvGetFreq(hdlGPU[i], &clock_freq[i]);
        NvGetPstates(hdlGPU[i], &pstates_info[i]);
    }

    switch(systype)
    {
        case 1:     printf("\nType: Laptop\n"); break;
        case 2:     printf("\nType: Desktop\n"); break;
        default:    printf("\nType: Unknown\n"); break;
    }

    printf("Name: %s\n", sysname);
    printf("Detected %d GPU(s)\n", nGPU);
    for(int i = 0; i < nGPU; i++)
    {
        printf("Settings for GPU %d\n", i);
        printf("VRAM: %dMB %s\n", memsize[i]/1024, get_memtype_string(memtype[i]));    
        printf("BIOS: %s\n", biosname[i]);
        free(biosname[i]);
        printf("\nGPU: %dMHz\n", (int)((clock_freq[i].domain[0].frequency) + ((pstates_info[i].pstates[0].clocks[0]).freqDelta_kHz.value))/1000);
        printf("VRAM: %dMHz\n", (int)((pstates_info[i].pstates[0].clocks[1]).data.single.freq_kHz)/get_memfreq_multiplier(memtype[i])/1000);
        printf("\nCurrent GPU OC: %dMHz\n", (int)((pstates_info[i].pstates[0].clocks[0]).freqDelta_kHz.value)/1000);
        printf("Current VRAM OC: %dMHz\n\n", (int)((pstates_info[i].pstates[0].clocks[1]).freqDelta_kHz.value)/get_memfreq_multiplier(memtype[i])/1000);
    }

    if(argc > 2)
    {
        int cur_gpu = atoi(argv[1]);
        if(cur_gpu >= nGPU)
        {
            printf("GPU%d not found!\nPlease specify a GPU within the range 0 to %d", cur_gpu, nGPU - 1);
            return 0;
        }
        
        printf("Changing settings for GPU%d", cur_gpu);

        if(argc > 2)
        {
            int gpufreq_mhz = atoi(argv[2]);
            int vramfreq_mhz;
            if(-250 <= gpufreq_mhz && gpufreq_mhz <= 250) {
                memset(&pstate_overclock, 0, sizeof(NV_GPU_PERF_PSTATES20_INFO_V1));
                pstate_overclock.version = 0x11c94; 
                pstate_overclock.numPstates = 1;
                pstate_overclock.numClocks = 1;
                pstate_overclock.pstates[0].clocks[0].domainId = 0; // GPU core clock domain
                pstate_overclock.pstates[0].clocks[0].freqDelta_kHz.value = gpufreq_mhz * 1000;
                if(argc > 3)
                {
                    vramfreq_mhz = atoi(argv[3]);
                    if(-250 <= vramfreq_mhz && vramfreq_mhz <= 250)
                    {
                        pstate_overclock.numClocks = 2;
                        pstate_overclock.pstates[0].clocks[1].domainId = 4; // VRAM clock domain
                        pstate_overclock.pstates[0].clocks[1].freqDelta_kHz.value = vramfreq_mhz * 1000 * get_memfreq_multiplier(memtype[cur_gpu]);
                    }
                    else
                    {
                        printf("\nVRAM frequency not in safe range (-250MHz to +250MHz).\n");
                        printf("Continuing with GPU overclock...\n");
                    }
                }

                if(NvSetPstates(hdlGPU[cur_gpu], &pstate_overclock))
                {
                    printf("\nGPU OC failed!\n");
                    if(argc > 3)
                        printf("VRAM OC failed!\n");
                }
                else
                {
                    printf("\nGPU OC OK: %d MHz\n", gpufreq_mhz);
                    if(argc > 3 && -250 <= vramfreq_mhz && vramfreq_mhz <= 250)
                        printf("VRAM OC OK: %d MHz\n", vramfreq_mhz);
                }
            } else {
                printf("\nGPU frequency not in safe range (-250MHz to +250MHz).\n");
                return 1;
            }
        }
    }

    NvUnload();
    return 0;
}
