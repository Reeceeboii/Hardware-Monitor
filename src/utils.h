//
// Created by reece on 16/12/2019.
//

#ifndef LINUX_SYSTEM_MONITOR_UTILS_H
#define LINUX_SYSTEM_MONITOR_UTILS_H

#include <gtk/gtk.h>

/**
 * Callback struct used to pass supposedly global params around without them having global scope.
 * Callbacks can annoyingly only have one parameter so I have to bundle multiple up to use more than 1
 */
struct callback_bundle{
    int mem_data_type;
    guint periodic_refresh_rate;
    struct window* win;
    struct labels* lab;
    struct data_unit_radios* radios;
    struct misc* misc;
    struct CPU_parsed* cpuParsed;
    struct mem_parsed* memParsed;
};

#endif //LINUX_SYSTEM_MONITOR_UTILS_H