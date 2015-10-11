#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include "reqchannel.H"

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

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
	RequestChannel chanel("control",RequestChannel::CLIENT_SIDE);

	return 0;
}