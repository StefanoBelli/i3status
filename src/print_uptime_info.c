// vim:ts=4:sw=4:expandtab
#include "i3status.h"
#include <yajl/yajl_gen.h>
#include <yajl/yajl_version.h>
#include <sys/sysinfo.h>

#define DAY(expr) (expr / 86400) 
#define HOUR(expr) ((expr % 86400) / 3600)
#define MINUTE(expr) ((expr % 3600) / 60)
#define SECOND(expr) (expr % 60)

//format: %day, %minute, %second, %hour
void print_uptime_info(yajl_gen json_gen, char *buffer, const char* format) {
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
        
        if (BEGINS_WITH(format + 1, "day")) {
            outwalk += sprintf(outwalk, "%ld", DAY(current.uptime));
            format += 3;
        } else if(BEGINS_WITH(format + 1, "minute")) {
            outwalk += sprintf(outwalk, "%02ld", MINUTE(current.uptime));
            format += 6;
        }  else if(BEGINS_WITH(format + 1, "second")) {
            outwalk += sprintf(outwalk, "%02ld", SECOND(current.uptime));
            format += 6;
        }  else if(BEGINS_WITH(format + 1, "hour")) {
            outwalk += sprintf(outwalk, "%02ld", HOUR(current.uptime));
            format += 4;
        }  
    }

    OUTPUT_FULL_TEXT(buffer);
#else
    OUTPUT_FULL_TEXT("missing implementation");
#endif
}
