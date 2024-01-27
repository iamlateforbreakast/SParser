/* Mutex.h */
#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "Types.h"
#include <pthread.h>

typedef struct Mutex
{
#ifndef WIN32
  pthread_mutex_t *mutex;
  pthread_cond_t cond; 
#else
  HANDLE mutex;
#endif
} Mutex;

PRIVATE void Mutex_new(Mutex * this, int initState);
PRIVATE void Mutex_delete(Mutex * this);
PRIVATE void Mutex_take(Mutex * this);
PRIVATE void Mutex_release(Mutex * this);
PRIVATE void Mutex_waitAvailability(Mutex * this);

PRIVATE void Mutex_new(Mutex * this, int initState)
{
#ifndef WIN32
//int pthread_mutex_init(this->mutex, const pthread_mutexattr_t *restrict attr);
//int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
//this->cond = PTHREAD_COND_INITIALIZER;
#else
//this->mutex = CreateMutexW(NULL, TRUE, NULL);      // Set
#endif
}

PRIVATE void Mutex_delete(Mutex * this)
{
//int pthread_mutex_destroy(this->mutex);
//int pthread_cond_destroy(pthread_cond_t *cond);
//CloseHandle(this->mutex);
  //if (hScreenMutex) CloseHandle(hScreenMutex);
  //if (hRunMutex) CloseHandle(hRunMutex);
}

PRIVATE void Mutex_take(Mutex * this)
{
#ifndef WIN32
//int pthread_mutex_lock(pthread_mutex_t *mutex);
#else
#endif
}

PRIVATE void Mutex_release(Mutex * this)
{
#ifndef WIN32
//int pthread_mutex_unlock(pthread_mutex_t *mutex)
//pthread_cond_signal(&condition); //wake up thread 1
#else
//ReleaseMutex(this->mutex); 
#endif
}

PRIVATE void Mutex_waitAvailability(Mutex * this)
{
  //dwWaitResult = WaitForSingleObject(this->mutex, INFINITE);  // no time-out interval
  //pthread_cond_wait(&cond, &lock);
  //int pthread_cond_timedwait(pthread_cond_t *restrict cond,
  //     pthread_mutex_t *restrict mutex,
  //     const struct timespec *restrict abstime
}
#endif /* _MUTEX_H_ */






////hScreenMutex = CreateMutexW(NULL, FALSE, NULL);  // Cleared
