#ifndef PORTABILITY_H
#define PORTABILITY_H

#ifdef _WIN32
    /* --- Windows Specifics --- */
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <io.h>
    #include <direct.h>

    /* Map POSIX functions to Windows equivalents */
    #define mkdir(path, mode) _mkdir(path)
    #define snprintf _snprintf
    
    /* Missing types in older MSVC C89 */
    typedef int ssize_t;

    typedef signed __int64 int64_t;
    typedef unsigned __int64 uint64_t;

    /* Signal compatibility */
    #define SIGHUP  1
    #define SIGKILL 9
    /* Note: Windows only natively supports SIGINT, SIGTERM, SIGABRT, SIGFPE, SIGILL, SIGSEGV */

    /* Empty macro for GCC attributes not supported by MSVC */
    #define UNUSED_PARAM(x) (void)(x)

#else
    /* --- POSIX Specifics --- */
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <sys/time.h>
    #include <stdint.h>

    #define UNUSED_PARAM(x) (void)(__attribute__((unused))x)
#endif

#endif /* PORTABILITY_H */
