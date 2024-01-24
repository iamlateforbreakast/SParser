/* Mutex.h */
#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "Types.h"

typedef struct Mutex
{
#ifndef WIN32
  pthread_mutex_t mutex;
#else
  HANDLE mutex;
#endif
} Mutex;

PRIVATE void Mutex_new(Mutex * this, int initState);
PRIVATE void Mutex_take(Mutex * this);
PRIVATE void Mutex_release(Mutex * this);

#endif /* _MUTEX_H_ */

//int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
//int pthread_mutex_lock(pthread_mutex_t *mutex);
//int pthread_mutex_unlock(pthread_mutex_t *mutex);
//int pthread_mutex_destroy(pthread_mutex_t *mutex);

////hScreenMutex = CreateMutexW(NULL, FALSE, NULL);  // Cleared
//hRunMutex = CreateMutexW(NULL, TRUE, NULL);      // Set