#ifndef COVERAGE_H
#define COVERAGE_H

#include <stdio.h>

#define ENABLE_COVERAGE_MEASUREMENT (0)
#define MAX_COVERAGE_POINTS 1024

/* Global tracking table */
extern unsigned char g_coverage_map[MAX_COVERAGE_POINTS];
extern const char* g_coverage_files[MAX_COVERAGE_POINTS];
extern int g_coverage_lines[MAX_COVERAGE_POINTS];
extern int g_coverage_count;

/* Macro placed at the start of functions or blocks to trace */
#if ENABLE_COVERAGE_MEASUREMENT
#define RECORD_COVERAGE(id) \
    do { \
        if (g_coverage_map[id] == 0) { \
            g_coverage_map[id] = 1; \
            g_coverage_files[id] = __FILE__; \
            g_coverage_lines[id] = __LINE__; \
            if (id >= g_coverage_count) { g_coverage_count = id + 1; } \
        } \
    } while(0)
#else
#define RECORD_COVERAGE(id)
#endif

void coverage_report(void);

#endif /* COVERAGE_H */
