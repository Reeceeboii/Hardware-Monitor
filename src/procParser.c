//
// Created by reece on 12/12/2019.
//

#include "procParser.h"
#include "utils.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Remove trailing whitespace from a string
 * @param token Pointer to the string that needs to be stripped
 * @return Pointer to stripped string
 */
char* trim_token(char* token) {
    char* end;
    while(isspace(*token)){
        ++token;
    }
    end = token + strlen(token) -1;
    while (end > token && isspace(*end)) {
        end = end - 1;
    }
    *(end+1) = '\0';
    return token;
}
/**
 * A memory value read has 'kb' on the end, this function removes it so it can be changed if need be.
 * @param s Pointer to the string that contains 'kb'.
*/
void trim_memory_size(char* s){
    s[strlen(s) - 3] = '\0';
}

/**
 * Calculate the percentage of system memory being used
 * @param cbb A pointer to the program's callback bundle
 * @return The percentage of memory being used
 */
gdouble calc_mem_used_percentage(struct callback_bundle* cbb){
    char* total = cbb->memParsed->total_mem_trimmed;
    char* available = cbb->memParsed->mem_available_trimmed;
    gdouble total_gd = atoi(total);
    gdouble available_gd = atoi(available);
    gdouble used_gd = total_gd - available_gd;
    gdouble used_percentage = used_gd / total_gd;
    return used_percentage;
}

/**
 * Calculate the percentage of system swap memory being used
 * @param cbb A pointer to the program's callback bundle
 * @return The percentage of memory being used
 */
gdouble calc_swap_used_percentage(struct callback_bundle* cbb){
    char* total = cbb->memParsed->total_swap_trimmed;
    char* available = cbb->memParsed->swap_available_trimmed;
    gdouble total_gd = atoi(total);
    gdouble available_gd = atoi(available);
    gdouble used_gd = total_gd - available_gd;
    gdouble used_percentage = used_gd / total_gd;
    return used_percentage;
}


/**
 * Takes a kb string read from proc and converts it to Mib and Gib
 * @param mib_destination Pointer to a char array to write the MiB conversion result to
 * @param gib_destination Pointer to a char array to write the GiB conversion result to
 */
void data_conversion(char* mib_destination, char* gib_destination){
    double mib = atof(mib_destination);
    double gib = atof(gib_destination);

    // carry out both Mib and Gib conversions
    for(int i = 0; i < MB; ++i){
        mib /= 1024;
    }
    for(int i = 0; i < GB; ++i){
        gib /= 1024;
    }

    // write calculations into the memParsed struct that owns that char pointers
    sprintf(mib_destination, "%.1f", mib);
    strncat(mib_destination, " MiB", DATABUF);
    sprintf(gib_destination, "%.1f", gib);
    strncat(gib_destination, " GiB", DATABUF);
}


/**
 * Read meminfo from proc and return a new mem_parsed struct representing that info
 * @return A new mem_parsed struct
 */
struct mem_parsed parse_mem(struct callback_bundle* cbb){
    char total_mem[DATABUF];
    char mem_available[DATABUF];
    char total_swap[DATABUF];
    char swap_available[DATABUF];
    FILE* proc_meminfo_p;
    char* line = NULL;
    char delim[] = ":";
    size_t len = 0;

    proc_meminfo_p = fopen("/proc/meminfo", "r");
    if(proc_meminfo_p == NULL){
        printf("%s", "Error - cannot read /proc/meminfo");
        exit(EXIT_FAILURE);
    }
    while((getline(&line, &len, proc_meminfo_p)) != -1) {
        char* delim_ptr = strtok(line, delim); // split on ':' (/proc/cpuinfo's delimiter)
        char* index0 = NULL;
        char* index1 = NULL;
        int ind = 0;
        while (delim_ptr != NULL) {
            delim_ptr = trim_token(delim_ptr);
            if (ind == 0) index0 = delim_ptr;
            else index1 = delim_ptr;
            ++ind;
            delim_ptr = strtok(NULL, delim);
        }
        if(strlen(index0) != 0 && strlen(index1) != 0){
            if(strcmp(index0, TOTAL_MEMORY) == 0){
                strncpy(total_mem, index1, DATABUF);
            } else if(strcmp(index0, TOTAL_SWAP) == 0){
                strncpy(total_swap, index1, DATABUF);
            } else if(strcmp(index0, AVAILABLE_MEMORY) == 0){
                strncpy(mem_available, index1, DATABUF);
            } else if(strcmp(index0, AVAILABLE_SWAP) == 0){
                strncpy(swap_available, index1, DATABUF);
            }
        }
    }
    fclose(proc_meminfo_p);
    struct mem_parsed mem;

    strncpy(mem.total_mem, total_mem, DATABUF);
    strncpy(mem.total_mem_trimmed, total_mem, DATABUF);
    strncpy(mem.total_mem_mib, total_mem, DATABUF);
    strncpy(mem.total_mem_gib, total_mem, DATABUF);
    trim_memory_size(mem.total_mem_trimmed);
    data_conversion(mem.total_mem_mib, mem.total_mem_gib);
    strncpy(mem.mem_available, mem_available, DATABUF);
    strncpy(mem.mem_available_trimmed, mem_available, DATABUF);
    strncpy(mem.mem_available_mib, mem_available, DATABUF);
    strncpy(mem.mem_available_gib, mem_available, DATABUF);
    trim_memory_size(mem.mem_available_trimmed);
    data_conversion(mem.mem_available_mib, mem.mem_available_gib);

    strncpy(mem.total_swap, total_swap, DATABUF);
    strncpy(mem.total_swap_trimmed, total_swap, DATABUF);
    strncpy(mem.total_swap_mib, total_swap, DATABUF);
    strncpy(mem.total_swap_gib, total_swap, DATABUF);
    trim_memory_size(mem.total_swap_trimmed);
    data_conversion(mem.total_swap_mib, mem.total_swap_gib);
    strncpy(mem.swap_available, swap_available, DATABUF);
    strncpy(mem.swap_available_trimmed, swap_available, DATABUF);
    strncpy(mem.swap_available_mib, swap_available, DATABUF);
    strncpy(mem.swap_available_gib, swap_available, DATABUF);
    trim_memory_size(mem.swap_available_trimmed);
    data_conversion(mem.swap_available_mib, mem.swap_available_gib);
    return mem;
}

/**
 * Read cpuinfo from proc and return a new CPU_parsed struct representing that info
 * @return A new CPU_parsed struct
 */
struct CPU_parsed parse_cpu() {
    char model_name[DATABUF];
    char cache_size[DATABUF];
    char core_count[DATABUF];
    gdouble thread_freq[THREADBUF];
    int thread_count = 0;
    FILE* proc_cpuinfo_p;
    char* line = NULL;
    char delim[] = ":";
    size_t len = 0;
    // open a file stream into /proc/cpuinfo
    proc_cpuinfo_p = fopen("/proc/cpuinfo", "r");

    if(proc_cpuinfo_p == NULL){
        printf("%s", "Error - cannot read /proc/cpuinfo");
        exit(EXIT_FAILURE);
    }

    while((getline(&line, &len, proc_cpuinfo_p)) != -1){
        char* delim_ptr = strtok(line, delim); // split on ':' (/proc/cpuinfo's delimiter)
        char* index0 = NULL;
        char* index1 = NULL;
        int ind = 0;
        while(delim_ptr != NULL){
            delim_ptr = trim_token(delim_ptr);
            if(ind == 0) index0 = delim_ptr;
            else index1 = delim_ptr;
            ++ind;
            delim_ptr = strtok(NULL, delim);
        }

        // buffer size is large enough to mitigate any non-null termination issues
        if(strlen(index0) != 0 && strlen(index1) != 0){
            if(strcmp(index0, MODEL_NAME) == 0){
                strncpy(model_name, index1, DATABUF);
            } else if(strcmp(index0, CACHE_SIZE) == 0){
                strcat(index1, " cache");
                strncpy(cache_size, index1, DATABUF);
            } else if(strcmp(index0, CORE_COUNT) == 0){
                strcat(index1, " cores");
                strncpy(core_count, index1, DATABUF);
            } else if(strcmp(index0, THREAD_FREQ) == 0){
                thread_freq[thread_count] = atof(index1);
                ++thread_count;
            }
        }
    }
    fclose(proc_cpuinfo_p);
    struct CPU_parsed result;
    strncpy(result.model_name, model_name, DATABUF);
    strncpy(result.cache_size, cache_size, DATABUF);
    strncpy(result.core_count, core_count, DATABUF);
    for(int i = 0; i < thread_count; ++i){
        result.thread_freq[i] = thread_freq[i];
    }
    result.thread_count = thread_count;
    return result;
}