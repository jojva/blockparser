#ifndef __ERRLOG_H__
    #define __ERRLOG_H__

    #include <errno.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>
    #include <stdlib.h>

    #include "callback.h"

    static inline void vError(
        const Callback *cb,
        int level,
        bool system,
        const char *format,
        va_list vaList
    )
    {
        fflush(stdout);
        fflush(stderr);

        bool info = (level==3);
        bool fatal = (level==0);
        bool warning = (level==2);

        const char *msgType =
            info    ? "info"    :
            fatal   ? "fatal"   :
            warning ? "warning" :
                      "error"
        ;

        fprintf(
            stderr,
            "%s: %s: %s%s",
            msgType,
            cb ? cb->name() : "\b\b",
            system ? strerror(errno) : "",
            system ? ": " : ""
        );

        vfprintf(
            stderr,
            format,
            vaList
        );

        fputc('\n', stderr);
        fflush(stdout);
        fflush(stderr);

        if(fatal) abort();
    }

    static inline void sysErr(
        const Callback *cb,
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(cb, 1, true, format, vaList);
        va_end(vaList);
    }

    static inline void sysErr(
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(NULL, 1, true, format, vaList);
        va_end(vaList);
    }

    static inline void errFatal(
        const Callback *cb,
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(cb, 0, false, format, vaList);
        va_end(vaList);
    }

    static inline void errFatal(
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(NULL, 0, false, format, vaList);
        va_end(vaList);
    }

    static inline void sysErrFatal(
        const Callback *cb,
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(cb, 0, true, format, vaList);
        va_end(vaList);
    }

    static inline void sysErrFatal(
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(NULL, 0, true, format, vaList);
        va_end(vaList);
    }

    static inline void warning(
        const Callback *cb,
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(cb, 2, false, format, vaList);
        va_end(vaList);
    }

    static inline void warning(
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(NULL, 2, false, format, vaList);
        va_end(vaList);
    }

    static inline void info(
        const Callback *cb,
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(cb, 3, false, format, vaList);
        va_end(vaList);
    }

    static inline void info(
        const char *format,
        ...
    )
    {
        va_list vaList;
        va_start(vaList, format);
            vError(NULL, 3, false, format, vaList);
        va_end(vaList);
    }

#endif // __ERRLOG_H__

