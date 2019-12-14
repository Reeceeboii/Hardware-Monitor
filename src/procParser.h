//
// Created by reece on 12/12/2019.
//

#ifndef LINUX_SYSTEM_MONITOR_PROCPARSER_H
#define LINUX_SYSTEM_MONITOR_PROCPARSER_H

#include <gtk/gtk.h>

// CPU
#define MODEL_NAME "model name"
#define CACHE_SIZE "cache size"
#define CORE_COUNT "cpu cores"
// MEM
#define TOTAL_MEMORY "MemTotal"
#define TOTAL_SWAP "SwapTotal"
#define FREE_MEMORY "MemFree"
#define FREE_SWAP "SwapFree"



#define DATABUF 80
#define FLAGBUF 1024 // accounting for possibly many CPU flags being encountered

struct CPU_parsed{
    char model_name[DATABUF];
    char core_count[DATABUF];
    char thread_count[DATABUF];
    char cache_size[FLAGBUF];
};

struct mem_parsed{
    char total_mem[DATABUF]; // present
    char total_swap[DATABUF]; // present
    char mem_used[DATABUF]; // calc
    char swap_used[DATABUF]; // calc
    char mem_free[DATABUF]; // present
    char swap_free[DATABUF]; // present
};

struct CPU_parsed parse_cpu();
struct mem_parsed parse_mem();
char* trim_token(char* token);


#endif //LINUX_SYSTEM_MONITOR_PROCPARSER_H
