//
// Created by reece on 12/12/2019.
//

#include "procParser.h"
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

char* data_conv(char* s, int t){
    if(t == 0){
        return s;
    }
    int val = atoi(s);
    for(int i = 0; i < t; ++i){
        val /= 1024;
    }
    char* s_val = NULL;
    sprintf(s_val, "%d", val);
    return s_val;
}

struct mem_parsed parse_mem(){
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
        printf("%s", "Error - cannot read /proc/cpuinfo");
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
    struct mem_parsed mem;
    strncpy(mem.total_mem, data_conv(total_mem, GB), DATABUF);
    strncpy(mem.total_swap, total_swap, DATABUF);
    strncpy(mem.mem_free, mem_free, DATABUF);
    strncpy(mem.swap_free, swap_free, DATABUF);
    return mem;
}

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