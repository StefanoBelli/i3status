// vim:ts=4:sw=4:expandtab
#include "i3status.h"
#include <yajl/yajl_gen.h>
#include <yajl/yajl_version.h>
#include <sys/sysinfo.h>

#define GBYTE(xstat) ((((float)xstat) / 1048576) / 1000)

//format: %avail, %total, %used
//Gigabytes
void print_swap_info(yajl_gen json_gen, char *buffer, const char* format) {
    char *outwalk = buffer;

#if defined(LINUX)
    struct sysinfo current;
    if(sysinfo(&current)) {
        OUTPUT_FULL_TEXT("sysinfo() failure");
        return;
    }

    for (; *format; format++) {
        if (*format != '%') {
            *(outwalk++) = *format;
            continue;
        }
        
        if (BEGINS_WITH(format + 1, "avail")) {
            outwalk += sprintf(outwalk, "%3.2f", GBYTE(current.freeswap));
            format += 5;
        } else if(BEGINS_WITH(format + 1, "total")) {
            outwalk += sprintf(outwalk, "%3.2f", GBYTE(current.totalswap));
            format += 5;
        }  else if(BEGINS_WITH(format + 1, "used")) {
            outwalk += sprintf(outwalk, "%3.2f", GBYTE(current.totalswap - current.freeswap));
            format += 4;
        } 
    }

    OUTPUT_FULL_TEXT(buffer);
#else
    OUTPUT_FULL_TEXT("missing implementation");
#endif
}

