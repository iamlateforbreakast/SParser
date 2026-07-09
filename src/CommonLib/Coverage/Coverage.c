#include "coverage.h"

unsigned char g_coverage_map[MAX_COVERAGE_POINTS] = {0};
const char* g_coverage_files[MAX_COVERAGE_POINTS] = {0};
int g_coverage_lines[MAX_COVERAGE_POINTS] = {0};
int g_coverage_count = 0;

void coverage_report(void) {
    int i;
    int hit = 0;
    int total = g_coverage_count;

    printf("\n=== RUNTIME COVERAGE REPORT ===\n");
    for (i = 0; i < total; i++) {
        if (g_coverage_files[i] != NULL) {
            if (g_coverage_map[i]) {
                printf("[ HIT ] %s : Line %d\n", g_coverage_files[i], g_coverage_lines[i]);
                hit++;
            } else {
                printf("[MISS ] %s : Line %d\n", g_coverage_files[i], g_coverage_lines[i]);
            }
        }
    }
    if (total > 0) {
        printf("Total Coverage: %.2f%%\n", ((double)hit / total) * 100.0);
    }
}
