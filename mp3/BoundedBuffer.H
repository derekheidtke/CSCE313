// definition of bounded buffer structure
#include <pthread.h>
#include <string>
#include <vector>
#include "Semaphore.H"

class BoundedBuffer{
	private:
		std::vector<std::string> space;
		int first;		// index of first element(next to be removed)
		int after_last;	// index of element after the last valid element
		int count;		// number of items in the buffer
		int MAX;		// max capacity of buffer
		Semaphore sema;
		pthread_cond_t not_full;
	public:
		BoundedBuffer(int); // initialized with n spaces

		void add(std::string);	// returns true if successful
		std::string remove();	// returns true if successful
};