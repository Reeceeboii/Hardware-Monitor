//
// Created by reece on 12/12/2019.
//

#include "procParser.h"
#include <stdio.h>
#include <ctype.h>

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

char* get_cpu_model_name(){
    FILE* proc_cpuinfo_p;
    char* line = NULL;
    char delim[] = ":";
    size_t len = 0;
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
        // extract each token from the delimiter split line, trim it
        while(delim_ptr != NULL){
            delim_ptr = trim_token(delim_ptr);
            if(ind == 0) index0 = delim_ptr;
            else index1 = delim_ptr;
            ++ind;
            delim_ptr = strtok(NULL, delim);
        }
        if(strlen(index0) != 0 && strlen(index1) != 0){
            if(strcmp(index0, MODEL_NAME) == 0){
                return index1;
            }
        }
    }
    free(line);
    fclose(proc_cpuinfo_p);
    return "Undefined";
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
        // extract each token from the delimiter split line, trim it
        while(delim_ptr != NULL){
            delim_ptr = trim_token(delim_ptr);
            if(ind == 0) index0 = delim_ptr;
            else index1 = delim_ptr;
            ++ind;
            delim_ptr = strtok(NULL, delim);
        }

        // buffer size is large enough to mitigate any non-null termination issues
        // -- no segfaults here :)
        if(strlen(index0) != 0 && strlen(index1) != 0){
            if(strcmp(index0, MODEL_NAME) == 0){
                strncpy(model_name, index1, DATABUF);
            } else if(strcmp(index0, CACHE_SIZE) == 0){
                strncpy(cache_size, index1, DATABUF);
            }
        }
    }
    fclose(proc_cpuinfo_p);
    struct CPU_parsed result;
    strncpy(result.model_name, model_name, DATABUF);
    strncpy(result.cache_size, cache_size, DATABUF);
    return result;
}