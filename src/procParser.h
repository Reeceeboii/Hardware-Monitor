//
// Created by reece on 12/12/2019.
//

#ifndef LINUX_SYSTEM_MONITOR_PROCPARSER_H
#define LINUX_SYSTEM_MONITOR_PROCPARSER_H

#include <gtk/gtk.h>
#define MODEL_NAME "model name"

struct CPU_parsed {
    char* model_name;
    char* core_count;
    char* thread_count;
    char* cache_size;
};

//typedef struct thread_frequency{
//    float threads[];
//};

struct CPU_parsed parse_cpu();
char* trim_token(char* token);
char* get_cpu_model_name();

#endif //LINUX_SYSTEM_MONITOR_PROCPARSER_H
