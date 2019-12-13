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
    gboolean model = FALSE;
    struct CPU_parsed result = { .model_name = "", .cache_size = "", .core_count = "", .thread_count = "" };
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
        if(strlen(index0) != 0 && strlen(index1) != 0 && !model) {
            if(strcmp(index0, MODEL_NAME) == 0){
                result.model_name = index1;
                model = TRUE;
                printf("%s is equal to %s\n", index0, MODEL_NAME);
            }
        }
    }
    free(line);
    fclose(proc_cpuinfo_p);
    return result;
}