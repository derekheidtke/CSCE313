#include "apue.h"
#include <pthread.h>
#include <iostream>

#include "Semaphore.H"
#include "BoundedBuffer.H"


BoundedBuffer* BUFFER;

// Global semaphore, accessible to all threads
Semaphore s(1);
int in = 0;

void * thread0(void * arg);
void * thread1(void * arg);

int main(void) {
	pthread_t tid0;
	pthread_t tid1;
	int err;

	BoundedBuffer tmp = BoundedBuffer(3);
	BUFFER = &tmp;

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
	pthread_create(&tid1, NULL, thread0, NULL);
	if ( err != 0 ){
		std::cerr << "\nCould not create thread 3.\n" << std::flush;
	}else{
		std::cerr << "\nCreated thread 3.\n" << std::flush;
	}
	for(int i = 0; i < 10; i++){
		std::cout << "." << std::flush;
		usleep(200000);
	}
	pthread_create(&tid1, NULL, thread1, NULL);
	if ( err != 0 ){
		std::cerr << "\nCould not create thread 4.\n" << std::flush;
	}else{
		std::cerr << "\nCreated thread 4.\n" << std::flush;
	}


	while( in != 0)	// wait until all are finished
		usleep(10);
	return 0;
}

void * thread0(void * arg) {
	in++;
	usleep(100);
	printf("\nThis is thread:%lx\n",(unsigned long)pthread_self() );

	BUFFER->add("test");

	// for(int i = 0; i < 10; i++){
	// 	std::cout << "." << std::flush;
	// 	usleep(200000);
	// }

	std::cout << "\n" << std::hex << pthread_self() << " Done.\n" << std::flush;
	in--;
}

void * thread1(void * arg) {
	in++;
	usleep(100);
	printf("\nThis is thread:%lx\n",(unsigned long)pthread_self() );

	std::string removed = BUFFER->remove();
	std::cout << "\nThread " << std::hex << pthread_self() << " : removed string: " << removed << "\n" << std::flush;

	// for(int i = 0; i < 10; i++){
	// 	std::cout << "." << std::flush;
	// 	usleep(200000);
	// }

	std::cout << "\n" << std::hex << pthread_self() << " Done.\n" << std::flush;
	in--;
}