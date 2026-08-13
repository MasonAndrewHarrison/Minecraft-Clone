#include "state.h"
#include <sys/resource.h>
#include <unistd.h>
#include <unistd.h>

State* initState(GLFWwindow* window, const GLFWvidmode* mode, bool startWithInfiniteWorldGen){

    State* state = malloc(sizeof(State));
    state->wireFrame = 0;
    state->needOfUpdate = 0;
    state->window = window;
    state->cam = cameraInit(mode->height, mode->width);
    state->infiniteWorldGen = startWithInfiniteWorldGen;
    state->tabWasPressed = 0;
    state->nearMaxOutOnRam = false;

    return state;
}



#ifdef __linux__
#include <stdio.h>
double getSystemMemoryPercent(){
    FILE* f = fopen("/proc/meminfo", "r");
    if (!f) return 0.0;
    long memTotal = 0, memAvailable = 0;
    char line[256];
    while (fgets(line, sizeof(line), f)){
        sscanf(line, "MemTotal: %ld kB", &memTotal);
        sscanf(line, "MemAvailable: %ld kB", &memAvailable);
    }
    fclose(f);
    if (memTotal == 0) return 0.0;
    return 100.0 * (1.0 - (double)memAvailable / (double)memTotal);
}

#elif defined(__APPLE__)
#include <mach/mach.h>
//UNTESTED CODE
double getSystemMemoryPercent(){
    mach_port_t host = mach_host_self();
    vm_size_t pageSize;
    host_page_size(host, &pageSize);

    vm_statistics64_data_t stats;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    host_statistics64(host, HOST_VM_INFO64, (host_info64_t)&stats, &count);

    long usedPages = stats.active_count + stats.wire_count + stats.compressor_page_count;
    long totalPages = usedPages + stats.free_count + stats.inactive_count;
    if (totalPages == 0) return 0.0;
    return 100.0 * (double)usedPages / (double)totalPages;
}

#elif defined(_WIN32)
#include <windows.h> 
//UNTESTED CODE
double getSystemMemoryPercent(){
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return (double)status.dwMemoryLoad;  
}
#endif