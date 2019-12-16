//
// Created by reece on 12/12/2019.
//

#include "procParser.h"
#include "main.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// remove trailing whitespace from a string
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
 * @param s The string that contains 'kb'.
*/
void trim_memory_size(char* s){
    s[strlen(s) - 3] = '\0';
}

/**
 * Given the memory size flag in the callback bundle, add the correct data type on the end of a memory string
 * @param s The string being edited
 * @param cbb A pointer to the program's callback bundle
 * @return The string with the correct data type appended on the end
 */
 /*
char* add_memory_size(char* s, struct callback_bundle* cbb){
    char* trimmed = trim_memory_size(s);
    char* converted = data_conv(trimmed, cbb);
    char appended[strlen(converted) + 15];
    strcat(appended, converted);
    switch(cbb->mem_data_type){
        case KB:
            strcat(appended, " KB");
            break;
        case MB:
            strcat(appended, " MB");
            break;
        case GB:
            strcat(appended, " GB");
            break;
    }
    char* ret = appended;
    return ret;
}
*/

 // TODO memfree confused with memavailable from proc - change this and labels to get correct percentages
gdouble calc_mem_used_percentage(struct callback_bundle* cbb){
    char* total = cbb->memParsed->total_mem;
    char* free = cbb->memParsed->mem_free;
    trim_memory_size(total);
    trim_memory_size(free);
    gdouble total_gd = atoi(total);
    gdouble free_gd = atoi(free);
    gdouble used_gd = total_gd - free_gd;
    gdouble used_percentage = used_gd / total_gd;
    return used_percentage;
}

char* data_conv(char* s, struct callback_bundle* cbb){
    if(cbb->mem_data_type) {
        int val = atoi(s);
        for (int i = 0; i < cbb->mem_data_type; ++i) {
            val /= 1024;
        }
        char converted[30];
        sprintf(converted, "%f", val);
        char* ret = converted;
        return ret;
    }
    return s;
}

/**
 * Read meminfo from proc and return a new mem_parsed struct representing that info
 * @return A new mem_parsed struct
 */
struct mem_parsed parse_mem(struct callback_bundle* cbb){
    char total_mem[DATABUF];
    char total_swap[DATABUF];
    char mem_free[DATABUF];
    char swap_free[DATABUF];
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
        char *delim_ptr = strtok(line, delim); // split on ':' (/proc/cpuinfo's delimiter)
        char *index0 = NULL;
        char *index1 = NULL;
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
                //strncpy(total_mem, add_memory_size(index1, cbb), DATABUF);
                strncpy(total_mem, index1, DATABUF);
            } else if(strcmp(index0, TOTAL_SWAP) == 0){
                strncpy(total_swap, index1, DATABUF);
            } else if(strcmp(index0, FREE_MEMORY) == 0){
                strncpy(mem_free, index1, DATABUF);
            } else if(strcmp(index0, FREE_SWAP) == 0){
                strncpy(swap_free, index1, DATABUF);
            }
        }
    }
    fclose(proc_meminfo_p);
    struct mem_parsed mem;
    strncpy(mem.total_mem, total_mem, DATABUF);
    strncpy(mem.total_swap, total_swap, DATABUF);
    strncpy(mem.mem_free, mem_free, DATABUF);
    strncpy(mem.swap_free, swap_free, DATABUF);
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
    char thread_count[DATABUF];
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
            }
        }
    }
    fclose(proc_cpuinfo_p);
    struct CPU_parsed result;
    strncpy(result.model_name, model_name, DATABUF);
    strncpy(result.cache_size, cache_size, DATABUF);
    strncpy(result.core_count, core_count, DATABUF);
    return result;
}