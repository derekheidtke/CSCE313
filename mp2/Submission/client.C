#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

#include "reqchannel.H"

//===========================================================

struct timeval tp_start; /* Used to compute elapsed time. */
struct timeval tp_end;

void print_time_diff(struct timeval * tp1, struct timeval * tp2) {
	/* Prints to stdout the difference, in seconds and museconds, between two
	 timevals. */

	long sec = tp2->tv_sec - tp1->tv_sec;
	long musec = tp2->tv_usec - tp1->tv_usec;
	if (musec < 0) {
		musec += 1000000;
		sec--;
	}
	printf(" [sec = %ld, musec = %ld] ", sec, musec);

}

const char * fake_request(const char *);


string int2string(int number) {
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

//===========================================================

int main(int argc, char * argv[]) {

	// fork and execute the dataserver from (current) parent
	pid_t PID;
	if( (PID = fork()) < 0 ) {  // error catching
		fprintf(stderr, "fork() error\n");
		return 1;					// ERROR: should not happen
	} else if ( PID == 0 ) {	// Child code block
		// execvp()
		if ( execvp("./dataserver", argv) < 0 ) {
			fprintf(stderr, "execvp() error\n");
			return 1;
		}
		printf("Should not print.\n");
	}

	// Parent continues with requests
	usleep(100000);
	RequestChannel chanel("control",RequestChannel::CLIENT_SIDE);	// construct client side pipe
	usleep(1000000);

	// test with "hello"
	string reply1 = chanel.send_request("hello");
	std::cout << "Reply to request 'hello' is '" << reply1 << "'" << std::endl << flush;

   	printf("=============TIMING=REQUEST========================\n");
		assert(gettimeofday(&tp_start, 0) == 0);
	// test with "hello"
	reply1 = chanel.send_request("data " + int2string(4) );
	std::cout << "Reply to request 'data 4' is '" << reply1 << "'" << std::endl << flush;	
		assert(gettimeofday(&tp_end, 0) == 0);
		printf("Time taken for server request: "); 
    	print_time_diff(&tp_start, &tp_end);
    	printf("\n");
   	printf("=============TIMING=REQUEST========================\n");


   	printf("=============TIMING=FAKE=REQUEST========================\n");
    	assert(gettimeofday(&tp_start, 0) == 0);
	// test with fake request
	reply1 = fake_request("This is a fake request.");
	std::cout << "Reply to request 'This is a fake request.' is '" << reply1 << "'" << std::endl << flush;	
		assert(gettimeofday(&tp_end, 0) == 0);
		printf("Time taken for fake request: "); 
    	print_time_diff(&tp_start, &tp_end);
    	printf("\n");
   	printf("=============TIMINGFAKE==REQUEST========================\n");

	// test close with "quit"
	reply1 = chanel.send_request("quit");
	std::cout << "Reply to request 'quit' is '" << reply1 << "'" << std::endl << flush;


	wait(&PID);
	return 0;
}


//===========================================================
const char* fake_request(const char * test_string) {
	const char* resp = "fake_response";
	return resp;
}
