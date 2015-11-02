// implementation of bounded buffer
#include "BoundedBuffer.H"
#include "Semaphore.H"
#include <string>
#include <iostream>

BoundedBuffer::BoundedBuffer(int n){
	first = 0;
	after_last = 0;
	count = 0;
	MAX = n;
	sema = Semaphore(1);
	for(int i = 0; i < MAX; i++) {
		space.push_back("!");			// "!" placeholder content; should never be seen
	}
}


void BoundedBuffer::add(std::string req){
	bool success_flag = 0;

	while( !success_flag ){					// repeat until successful
		sema.P();							// only one thread allowed in buffer at a time
		if( count < MAX ) {					// can't add item if buffer is full
			
			space[after_last] = req;		// insert new request
			after_last = (after_last+1)%MAX;
			count++;						// update number of filled spaces
			success_flag = true;			// flag successfull
		} else {
			success_flag = false;
		}
		sema.V();
	}
}
std::string BoundedBuffer::remove(){
	bool success_flag = 0;
	std::string removed;

	while( !success_flag ) {			// repeat until successful
		sema.P();						// only one thread allowed in buffer at a time
		if( count > 0 ) {				// can't remove item if buffer is empty
			removed = space[first];		// set return variable
			space[first] = "!";			// delete old space
			if( first == after_last ) {
				// do nothing
			} else {
				first = (first+1)%MAX;	// increment
			}
			count--;					// update number of filled spaces
			success_flag = true;		// flag successful
		} else {
			success_flag = 0;
		}
		sema.V();
	}

	return removed;
}