#if defined(_POSIX_VERSION) && (!defined(_POSIX_BARRIERS) || (_POSIX_BARRIERS == -1))

#include <errno.h>
#include "pthread_barrier.h"

int
pthread_barrier_destroy(pthread_barrier_t *barrier) {
	int err;
	if(barrier == NULL) return EINVAL;
	if((err = pthread_cond_destroy(&barrier->cond)) != 0)   return err;
	if((err = pthread_mutex_destroy(&barrier->mutex)) != 0) return err;
	barrier->waits = 0;
	barrier->count = 0;
	return 0;
}

int
pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned count) {
	int err;
	void *cattr = NULL;
	void *mattr = NULL;
	if(barrier == NULL) return EINVAL;
	if(attr != NULL) {
		cattr = (void *) &attr->cattr;
		mattr = (void *) &attr->mattr;
	}
	if((err = pthread_cond_init(&barrier->cond, cattr)) != 0)   return err;
	if((err = pthread_mutex_init(&barrier->mutex, mattr)) != 0) return err;
	barrier->waits = 0;
	barrier->count = count;
	return 0;
}

int
pthread_barrier_wait(pthread_barrier_t *barrier) {
	int err;
	if(barrier == NULL) return EINVAL;
	if((err = pthread_mutex_lock(&barrier->mutex)) != 0) return err;
	barrier->waits++;
	do {
		if(barrier->waits >= barrier->count)
			break;
		if((err = pthread_cond_wait(&barrier->cond, &barrier->mutex)) != 0) {
			pthread_mutex_unlock(&barrier->mutex);
			return err;
		}
	} while(1);
	if((err = pthread_mutex_unlock(&barrier->mutex)) != 0) return err;
	if((err = pthread_cond_signal(&barrier->cond)) != 0)   return err;
	return 0;
}

int
pthread_barrierattr_destroy(pthread_barrierattr_t *attr) {
	int err;
	if(attr == NULL) return EINVAL;
	if((err = pthread_condattr_destroy(&attr->cattr)) != 0)  return err;
	if((err = pthread_mutexattr_destroy(&attr->mattr)) != 0) return err;
	return 0;
}

int
pthread_barrierattr_init(pthread_barrierattr_t *attr) {
	int err;
	if(attr == NULL) return EINVAL;
	if((err = pthread_condattr_init(&attr->cattr)) != 0)  return err;
	if((err = pthread_mutexattr_init(&attr->mattr)) != 0) return err;
	return 0;
}

int
pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared) {
	int err;
	if(attr == NULL || pshared == NULL) return EINVAL;
	if((err = pthread_condattr_getpshared(&attr->cattr, pshared)) != 0)
		return err;
	if((err = pthread_mutexattr_getpshared(&attr->mattr, pshared)) != 0)
		return err;
	return 0;
}

int
pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared) {
	int err;
	if(attr == NULL) return EINVAL;
	if((err = pthread_condattr_setpshared(&attr->cattr, pshared)) != 0)
		return err;
	if((err = pthread_mutexattr_setpshared(&attr->mattr, pshared)) != 0)
		return err;
	return 0;
}

#endif
