#ifndef BKTRACE_H
#define BKTRACE_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static void debugtrace(char* fmtstr, ...) {
    FILE* file = fopen("gctrace.out", "a");
    va_list args;
    va_start(args, fmtstr);
    vfprintf(file, fmtstr, args);
    va_end(args);
    fclose(file);
}

/* Macros to log collector startup */
#define TRACE_INIT_START() \
    debugtrace("init_start, %ld, %d\n", clock(), CLOCKS_PER_SEC)

#define TRACE_INIT_END() \
    debugtrace("init_end, %ld\n", clock())

/* Macros to log collector shutdown */
#define TRACE_DEINIT_START() \
    debugtrace("deinit_start, %ld\n", clock())

#define TRACE_DEINIT_END() \
    debugtrace("deinit_end, %ld\n", clock())

/* Macros to log collection cycle */
#define TRACE_COLLECT_START() \
    debugtrace("collect_start, %ld\n", clock())

#define TRACE_COLLECT_END() \
    debugtrace("collect_end, %ld\n", clock())

/* Macros to log an allocation using the collector */
#define TRACE_ALLOC_START(ptr,size) \
    debugtrace("alloc_start, %ld, %p, %ld\n", clock(), ptr, size)

#define TRACE_ALLOC_END(ptr,size) \
    debugtrace("alloc_end, %ld, %p, %ld\n", clock(), ptr, size)

/* Macro to log an allocation using the collector */
#define TRACE_DEALLOC_START(ptr) \
    debugtrace("dealloc_start, %ld, %p\n", clock(), ptr)

#define TRACE_DEALLOC_END(ptr) \
    debugtrace("dealloc_end, %ld, %p\n", clock(), ptr)

/* Macro to log an reference increment using the collector */
#define TRACE_ADDREF_START(ptr) \
    debugtrace("addref_start, %ld, %p\n", clock(), ptr)

#define TRACE_ADDREF_END(ptr) \
    debugtrace("addref_end, %ld, %p\n", clock(), ptr)

/* Macro to log an reference decrement using the collector */
#define TRACE_DELREF_START(ptr) \
    debugtrace("delref_start, %ld, %p\n", clock(), ptr)

#define TRACE_DELREF_END(ptr) \
    debugtrace("delref_end, %ld, %p\n", clock(), ptr)

#endif
