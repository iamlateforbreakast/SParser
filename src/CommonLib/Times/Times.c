/* Times.c */

/*
//  Windows
#ifdef _WIN32
#include <Windows.h>
double get_wall_time() {
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq)) {
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time)) {
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time() {
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return
			(double)(d.dwLowDateTime |
				((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else {
		//  Handle error
		return 0;
	}
}
//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
long double get_wall_time() {
	struct timeval time;
	if (gettimeofday(&time, NULL)) {
		//  Handle error
		return 0;
	}
	return (long double)time.tv_sec + (long double)time.tv_usec/1000000;
}
long double get_cpu_time() {
	return (long double)clock() / CLOCKS_PER_SEC;
}
#endif
#endif
*/

PUBLIC uint64_t Time_getNow(void) {
#ifdef _WIN32
    static LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    
    /* Initialize frequency once */
    if (frequency.QuadPart == 0) {
        QueryPerformanceFrequency(&frequency);
    }
    
    QueryPerformanceCounter(&counter);
    
    /* Convert to microseconds: (Counter * 1,000,000) / Frequency */
    /* We use multiplication first for precision, but watch for overflow if 
       uptime is extremely high (years). */
    return (uint64_t)((counter.QuadPart * 1000000) / frequency.QuadPart);

#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    /* Convert seconds and microseconds to a single microsecond value */
    return (uint64_t)(tv.tv_sec) * 1000000 + (uint64_t)(tv.tv_usec);
#endif
}
