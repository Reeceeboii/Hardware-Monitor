//
// Created by reece on 12/12/2019.
//

#ifndef LINUX_SYSTEM_MONITOR_PROCPARSER_H
#define LINUX_SYSTEM_MONITOR_PROCPARSER_H

#include "utils.h"
#include <gtk/gtk.h>

// CPU
#define MODEL_NAME "model name"
#define CACHE_SIZE "cache size"
#define CORE_COUNT "cpu cores"
// MEM
#define TOTAL_MEMORY "MemTotal"
#define TOTAL_SWAP "SwapTotal"
#define AVAILABLE_MEMORY "MemAvailable"
#define FREE_SWAP "SwapFree"

#define DATABUF 80
#define FLAGBUF 1024 // accounting for possibly many CPU flags being encountered

// from kb, how many times you need to divide by 1024 to reach some other data units
enum data_sizes { GB = 2, MB = 1, KB = 0 };

struct CPU_parsed{
    char model_name[DATABUF];
    char core_count[DATABUF];
    char thread_count[DATABUF];
    char cache_size[FLAGBUF];
};

struct mem_parsed{
    // total memory in the system
    char total_mem[DATABUF];
    char total_mem_trimmed[DATABUF];
    char total_mem_mib[DATABUF];
    char total_mem_gib[DATABUF];
    // the system's current free memory
    char mem_available[DATABUF];
    char mem_available_trimmed[DATABUF];
    char mem_available_mib[DATABUF];
    char mem_available_gib[DATABUF];

    char total_swap[DATABUF];
    char swap_free[DATABUF];
};

struct CPU_parsed parse_cpu();
struct mem_parsed parse_mem(struct callback_bundle* cbb);
char* trim_token(char* token);
void data_conversion(char* mib_destination, char* gib_destination);
void trim_memory_size(char* s);
gdouble calc_mem_used_percentage(struct callback_bundle* cbb);

#endif //LINUX_SYSTEM_MONITOR_PROCPARSER_H