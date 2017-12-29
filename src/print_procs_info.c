// vim:ts=4:sw=4:expandtab
#include "i3status.h"
#include <yajl/yajl_gen.h>
#include <yajl/yajl_version.h>
#include <sys/sysinfo.h>

//format: %procs
void print_procs_info(yajl_gen json_gen, char *buffer, const char* format) {
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
        
        if (BEGINS_WITH(format + 1, "procs")) {
            outwalk += sprintf(outwalk, "%d", current.procs);
            format += 5;
        } 
    }

    OUTPUT_FULL_TEXT(buffer);
#else
    OUTPUT_FULL_TEXT("missing implementation");
#endif
}
