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
#define THREAD_FREQ "cpu MHz"
// MEM
#define TOTAL_MEMORY "MemTotal"
#define AVAILABLE_MEMORY "MemAvailable"
#define TOTAL_SWAP "SwapTotal"
#define AVAILABLE_SWAP "SwapFree"

// some misc constants
#define DATABUF 80
#define THREADBUF 32
#define MAX_SUPPORTED_THREADS 32 // this is reasonable for now
#define FLAGBUF 1024 // accounting for possibly many CPU flags being encountered

// from kib, how many times you need to divide by 1024 to reach some other data units
enum data_sizes { GB = 2, MB = 1, KB = 0 };

struct CPU_parsed{
    int thread_count;
    char model_name[DATABUF];
    char core_count[DATABUF];
    char cache_size[FLAGBUF];
    gdouble thread_freq[THREADBUF];
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
    // total swap memory in the system
    char total_swap[DATABUF];
    char total_swap_trimmed[DATABUF];
    char total_swap_mib[DATABUF];
    char total_swap_gib[DATABUF];
    // the system's current free swap memory
    char swap_available[DATABUF];
    char swap_available_trimmed[DATABUF];
    char swap_available_mib[DATABUF];
    char swap_available_gib[DATABUF];
};

struct CPU_parsed parse_cpu();
struct mem_parsed parse_mem(struct callback_bundle* cbb);
char* trim_token(char* token);
void data_conversion(char* mib_destination, char* gib_destination);
void trim_memory_size(char* s);
gdouble calc_mem_used_percentage(struct callback_bundle* cbb);
gdouble calc_swap_used_percentage(struct callback_bundle* cbb);

#endif //LINUX_SYSTEM_MONITOR_PROCPARSER_H