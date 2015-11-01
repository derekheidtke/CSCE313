
// Implementation of Semaphore class with mutexes and condition variables.
#include "Semaphore.H"
#include <pthread.h>

/* -- CONSTRUCTOR AND DESTRUCTOR */
Semaphore::Semaphore(int _val) {
	m 		= (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	c 		= (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	value 	= _val;
	pthread_mutex_unlock(&m);	// unneccesary?
}

Semaphore::~Semaphore() {

}

/* -- SEMAPHORE OPERATIONS */

// Semaphore P() operation: blocking
int Semaphore::P() {
	int lock_error = 0;
	int unlock_error = 0;
	int wait_error = 0;

	if( lock_error = pthread_mutex_lock(&m) )		// lock mutex
		return lock_error;
	while ( (value <= 0) && !wait_error )
		wait_error = pthread_cond_wait(&c, &m);		// wait until value > 0
	if( wait_error )
		return wait_error;
	value--;										// decrement value
	if( unlock_error = pthread_mutex_unlock(&m) )	// unlock mutex
		return unlock_error;

	return lock_error + unlock_error + wait_error;
}

int Semaphore::V() {
	int lock_error = 0;
	int unlock_error = 0;
	int signal_error = 0;

	if( lock_error = pthread_mutex_lock(&m) )		// lock mutex
		return lock_error;
	value++;										// increment value
	signal_error = pthread_cond_signal(&c);			// dignal to unblock a waiting thread
	if( signal_error )
		return signal_error;
	if( unlock_error = pthread_mutex_unlock(&m) )	// unlock mutex
		return unlock_error;
	
	return lock_error + unlock_error + signal_error;
}