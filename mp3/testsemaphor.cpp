#include "apue.h"
#include <pthread.h>
#include <iostream>

#include "Semaphore.H"

// Global semaphore, accessible to all threads
Semaphore s(1);
int in = 0;

void * thread0(void * arg);

int main(void) {
	pthread_t tid0;
	pthread_t tid1;
	int err;

	err = pthread_create(&tid0, NULL, thread0, NULL);
	if ( err != 0 ){
		std::cerr << "\nCould not create thread 0.\n" << std::flush;
		err = 0;
	}else{
		std::cerr << "\nCreated thread 0.\n" << std::flush;
	}
	pthread_create(&tid1, NULL, thread0, NULL);
	if ( err != 0 ){
		std::cerr << "\nCould not create thread 1.\n" << std::flush;
	}else{
		std::cerr << "\nCreated thread 1.\n" << std::flush;
	}
	pthread_create(&tid1, NULL, thread0, NULL);
	if ( err != 0 ){
		std::cerr << "\nCould not create thread 2.\n" << std::flush;
	}else{
		std::cerr << "\nCreated thread 2.\n" << std::flush;
	}


	while( in != 0)
		usleep(1000);
	return 0;
}

void * thread0(void * arg) {
	in++;
	usleep(100);
	s.P();
	printf("\nThis is thread:%lx\n",(unsigned long)pthread_self() );
	for(int i = 0; i < 10; i++){
		std::cout << s.get_value() << std::flush;
		usleep(200000);
	}
	std::cout << "\nDone.\n" << std::flush;
	s.V();
	in--;
}