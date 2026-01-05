#ifndef __PTHREAD_EXTRA__
#define __PTHREAD_EXTRA__

#if defined(_POSIX_VERSION) && (!defined(_POSIX_BARRIERS) || (_POSIX_BARRIERS == -1))

#include <unistd.h>
#include <pthread.h>

/* an emulated implementation for pthread barrier */

typedef struct pthread_barrier_s {
	pthread_cond_t  cond;
	pthread_mutex_t mutex;
	unsigned waits, count;
}	pthread_barrier_t;

typedef struct pthread_baerrierattr_s {
	pthread_condattr_t  cattr;
	pthread_mutexattr_t mattr;
}	pthread_barrierattr_t;

int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned count);
int pthread_barrier_wait(pthread_barrier_t *barrier);

int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int pthread_barrierattr_init(pthread_barrierattr_t *attr);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);

#endif	/* _POSIX_BARRIERS */

#endif /* __PTHREAD_EXTRA__ */
